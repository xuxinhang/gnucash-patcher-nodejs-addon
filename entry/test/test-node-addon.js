const fs = require('fs');
const path = require('path');

const addon = require('../../build/Debug/node-addon.node');
// const addon = require('bindings')('node-addon');

console.log(addon.hello());
// console.log(addon.pat());

const data = JSON.parse(fs.readFileSync(path.resolve(__dirname, './_sample_data.json'), 'utf-8'));
const result = addon.pat('file:///mnt/e/example.gnucash', data);

console.log(result);

