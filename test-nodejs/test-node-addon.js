const fs = require('fs');
const path = require('path');

// const addon = require('../../build/entry/node-addon.node');
const addon = require('bindings')('node-addon');

console.log(addon.hello());
// console.log(addon.pat());

const data = JSON.parse(fs.readFileSync(path.resolve(__dirname, './_sample_data.json'), 'utf-8'));

const sourceGncFilePath = 'file:///mnt/e/example.gnucash';
// const targetGncFilePath = 'file:///mnt/e/example-out.gnucash';
const result = addon.pat(data, sourceGncFilePath);
console.log(result);

