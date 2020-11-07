#include <napi.h>
#include <string>
#include <list>
#include <iostream>

#include "config.h"
#include "qof.h"
#include "Account.h"

#include "book-patcher.hpp"

#define SAFE_NAPI_OBJECT_GET_STRING(o, key, zero) \
  (o.Has(key) ? o.Get(key).As<Napi::String>().Utf8Value() : (zero))

#define SAFE_NAPI_OBJECT_GET_INT32(o, key, zero) \
  (o.Has(key) ? o.Get(key).As<Napi::Number>().Int32Value() : (zero))


Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world1");
}

Napi::Boolean pat_method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Wrong number of arguments")
      .ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  if (!info[1].IsString() || !info[0].IsArray()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return Napi::Boolean::New(env, false);
  }

  std::string sourceURI = info[1].As<Napi::String>().Utf8Value();
  // std::string targetURI = info[2].As<Napi::String>().Utf8Value();
  Napi::Array narray_records = info[0].As<Napi::Array>();
  std::cout << "The book file is " << sourceURI << std::endl;

  init_gnc_modules();

  QofSession *sess = qof_session_new();
  qof_session_begin(sess, sourceURI.c_str(), true, false, true);
  qof_session_load(sess, [](const char *msg, double perc) -> void {
    // std::cout << "[" << (int)perc << "%] book loading: " << (msg ? msg : "") << std::endl;
  });

  QofBook* book = qof_session_get_book(sess);
  Account* root_account = gnc_book_get_root_account(book);
  std::cout
  << "The root account's GUID is "
  << guid_to_string(xaccAccountGetGUID(root_account))
  << std::endl;

  BookPatcher patcher(book);
  unsigned int i;
  TransferRecord rec;

  for (i = 0; i < narray_records.Length(); i++) {
    Napi::Value el = narray_records.Get(i);
    if (!el.IsObject()) continue;

    Napi::Object ob = el.As<Napi::Object>();

    std::string temp_description = SAFE_NAPI_OBJECT_GET_STRING(ob, "description", "");
    std::string temp_sourceAccount = SAFE_NAPI_OBJECT_GET_STRING(ob, "sourceAccount", "CAMPUS_CARD");
    std::string temp_targetAccount = SAFE_NAPI_OBJECT_GET_STRING(ob, "targetAccount", "MEAL");
    std::string temp_reconcileState = SAFE_NAPI_OBJECT_GET_STRING(ob, "reconcileState", "c");

    rec.amount = SAFE_NAPI_OBJECT_GET_INT32(ob, "amount", 0);
    rec.denom = SAFE_NAPI_OBJECT_GET_INT32(ob, "denom", 100);
    rec.description = (char*)temp_description.c_str();
    rec.sourceAccount = (char*)temp_sourceAccount.c_str();
    rec.targetAccount = (char*)temp_targetAccount.c_str();
    rec.timestamp = SAFE_NAPI_OBJECT_GET_INT32(ob, "timestamp", 0);
    rec.reconcileState = temp_reconcileState.front();

    // std::cout
    // << "\ndesc as string: " << desc
    // << "\ndesc as charp: "  << desc.c_str()
    // << std::endl;

    // rec.description = (char *)desc.c_str();

    std::cout
    << "=== Current record ==="
    << "\namount: "        << rec.amount
    << "\ndenom: "         << rec.denom
    << "\ndescription: "   << rec.description
    << "\nsourceAccount: " << rec.sourceAccount
    << "\ntargetAccount: " << rec.targetAccount
    << "\ntimestamp: "     << rec.timestamp
    << "\n\n"
    << std::endl;

    patcher.addTransfer(&rec);
  }

  qof_session_save(sess, [](const char *msg, double perc) -> void {
    // std::cout << "[" << (int)perc << "%] book saving: " << (msg || "") << std::endl;
  });
  qof_session_end(sess);
  qof_session_destroy(sess);

  return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, Method));
  exports.Set(Napi::String::New(env, "pat"), Napi::Function::New(env, pat_method));
  return exports;
}

NODE_API_MODULE(hello, Init)


