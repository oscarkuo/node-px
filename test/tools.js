var fs = require('fs');
var px = require('../px');
var path = require('path');

module.exports.makePxFilePath = function(prefix) {
  return path.join(
    'test', prefix + '.' + Math.random().toString(36).substring(7) + ".test.px");
}

module.exports.deletePxFile = function(pxDoc, pxFilePath) {
  if (pxDoc) pxDoc.close();
  if (fs.existsSync(pxFilePath)) fs.unlinkSync(pxFilePath);
}

module.exports.unknownType = { name: 'aaa', size: 17, digits: 7, type: 'xxx' };
module.exports.missingName = { type: 'pxfBCD', size: 17, digits: 7 };
module.exports.missingType = { name: 'aaa', size: 17, digits: 7 };
module.exports.missingSize = { name: 'aaa', type: 'pxfBCD', digits: 7 };
module.exports.missingDigitsBcd = { name: 'aaa', size: 17, type: 'pxfBCD' };
module.exports.missingDigitsNotBcd = { name: 'aaa', size: 17, type: 'pxfAlpha' };
module.exports.wrongTypeName = { name: 111, type: 'pxfBCD', size: 17, digits: 7 };
module.exports.wrongTypeType = { name: 'aaa', type: 111, size: 17, digits: 7 };
module.exports.wrongTypeSize = { name: 'aaa', type: 'pxfBCD', size: 'aaa', digits: 7 };
module.exports.wrongTypeDigitsBcd = { name: 'aa', type: 'pxfBCD', size: 17, digits: 'aaa' };
module.exports.validAlpha = { name: 'alphaField', type: 'pxfAlpha', size: 20 };
module.exports.validShort = { name: 'shortField', type: 'pxfShort' };
module.exports.validLong = { name: 'longField', type: 'pxfLong' };
module.exports.validDate = { name: 'dateField', type: 'pxfDate' };
module.exports.validAutoInc = { name: 'autoIncField', type: 'pxfAutoInc' };
module.exports.validNumber = { name: 'numberField', type: 'pxfNumber' };
module.exports.validCurrency = { name: 'currencyField', type: 'pxfCurrency' };
module.exports.validLogical = { name: 'logicalField', type: 'pxfLogical' };
module.exports.validTime = { name: 'timeField', type: 'pxfTime' };
module.exports.validTimestamp = { name: 'timestampField', type: 'pxfTimestamp' };
module.exports.validBCD = { name: 'bcdField', type: 'pxfBCD', size: 17, digits: 7 };
module.exports.notAnObject = 1;

module.exports.validAllTypesInput = [
  { name: 'aaa', type: 'pxfAlpha', size: 17 },
  { name: 'bbb', type: 'pxfDate' },
  { name: 'ccc', type: 'pxfShort' },
  { name: 'ddd', type: 'pxfLong' },
  { name: 'eee', type: 'pxfAutoInc' },
  { name: 'fff', type: 'pxfNumber' },
  { name: 'ggg', type: 'pxfCurrency' },
  { name: 'hhh', type: 'pxfLogical' },
  { name: 'iii', type: 'pxfTime' },
  { name: 'jjj', type: 'pxfTimestamp' },
];

module.exports.validAllTypesOutput = [
  { name: 'aaa', type: 'pxfAlpha', size: 17, digits: 0 },
  { name: 'bbb', type: 'pxfDate', size: 4, digits: 0 },
  { name: 'ccc', type: 'pxfShort', size: 2, digits: 0 },
  { name: 'ddd', type: 'pxfLong', size: 4, digits: 0 },
  { name: 'eee', type: 'pxfAutoInc', size: 4, digits: 0 },
  { name: 'fff', type: 'pxfNumber', size: 8, digits: 0 },
  { name: 'ggg', type: 'pxfCurrency', size: 8, digits: 0 },
  { name: 'hhh', type: 'pxfLogical', size: 1, digits: 0 },
  { name: 'iii', type: 'pxfTime', size: 4, digits: 0 },
  { name: 'jjj', type: 'pxfTimestamp', size: 8, digits: 0 },
];


