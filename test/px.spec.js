var fs = require('fs');
var px = require('../px');
var tools = require('./tools.js');

describe("px.make tests:", function () {
  var pxFilePath;
  var pxDocument;

  beforeEach(function() {
    pxFilePath = tools.makePxFilePath('px.make');
  });

  afterEach(function() {
    tools.deletePxFile(pxDocument, pxFilePath);
  });

  it("px.make does not throw for valid Alpha field", function() {
    var tf = function () { pxDocument = px.make([tools.validAlpha], pxFilePath) }; 
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Time field", function() {
    var tf = function () { pxDocument = px.make([tools.validTime], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Timestamp field", function() {
    var tf = function () { pxDocument = px.make([tools.validTimestamp], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Short field", function() {
    var tf = function () { pxDocument = px.make([tools.validShort], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Long field", function() {
    var tf = function () { pxDocument = px.make([tools.validLong], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Currency field", function() {
    var tf = function () { pxDocument = px.make([tools.validCurrency], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Logical field", function() {
    var tf = function () { pxDocument = px.make([tools.validLogical], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Number field", function() {
    var tf = function () { pxDocument = px.make([tools.validNumber], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid Date field", function() {
    var tf = function () { pxDocument = px.make([tools.validDate], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make does not throw for valid AutoInc field", function() {
    var tf = function () { pxDocument = px.make([tools.validAutoInc], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make creates a NpxDocumentument", function() {
    pxDocument = px.make([tools.validBCD], pxFilePath);
    expect(pxDocument.toString()).toEqual("[object NpxDocument]");
  });

  it("px.make Creates a .px file on the file system", function() {
    pxDocument = px.make([tools.validBCD], pxFilePath);
    expect(fs.existsSync(pxFilePath)).toEqual(true);
  });

  it("px.make Will not throws error when given correct arguments", function() {
    var tf = function() { pxDocument = px.make([tools.validBCD], pxFilePath); };
    expect(tf).not.toThrow();
  });

  it("px.make throws error when given no argument", function() {
    var tf = function() { pxDocument = px.make(); };
    expect(tf).toThrow("Bad argument for px.make(fields, path)");
  });

  it("px.make throws error when given only 1 argument", function() {
    var tf = function() { pxDocument = px.make([tools.validBCD]); };
    expect(tf).toThrow("Bad argument for px.make(fields, path)")
  });

  it("px.make throws error when 1st parameter is not an array", function() {
    var tf = function() { pxDocument = px.make(1, "aaa"); };
    expect(tf).toThrow("[1] is not of type [array]");
  });

  it("px.make throws error when given field definition is not an object", function() {
    var tf = function() { pxDocument = px.make([tools.notAnObject], pxFilePath); };
    expect(tf).toThrow("[1] is not of type [object]");
  });

  it("px.make throws error when given [name] property is not a string", function() {
    var tf = function() { pxDocument = px.make([tools.wrongTypeName], pxFilePath); };
    expect(tf).toThrow("[111] is not of type [string]");
  });

  it("px.make throws error when given [type] property is not a string", function() {
    var tf = function() { pxDocument = px.make([tools.wrongTypeType], pxFilePath); };
    expect(tf).toThrow("[111] is not of type [string]");
  });

  it("px.make throws error when given [size] property is not a integer", function() {
    var tf = function() { pxDocument = px.make([tools.wrongTypeSize], pxFilePath); };
    expect(tf).toThrow("[aaa] is not of type [integer]");
  });

  it("px.make throws error when given [digits] property is not a integer", function() {
    var tf = function() { pxDocument = px.make([tools.wrongTypeDigitsBcd], pxFilePath); };
    expect(tf).toThrow("[aaa] is not of type [integer]");
  });

  it("px.make throws error when missing [name] property", function() {
    var tf = function() { pxDocument = px.make([tools.missingName], pxFilePath); };
    expect(tf).toThrow("Property [name] is not defined in object [#<Object>]")
  });

  it("px.make throws error when missing [type] property", function() {
    var tf = function() { pxDocument = px.make([tools.missingType], pxFilePath); };
    expect(tf).toThrow("Property [type] is not defined in object [#<Object>]");
  });

  it("px.make throws error when missing [size] property", function() {
    var tf = function() { pxDocument = px.make([tools.missingSize], pxFilePath); };
    expect(tf).toThrow("Property [size] is not defined in object [#<Object>]");
  });

  it("px.make throws error when given unknown field type", function() {
    var tf = function() { pxDocument = px.make([tools.unknownType], pxFilePath); };
    expect(tf).toThrow("Unknown pxfield type [xxx]");
  });

  it("px.make throws error when missing [digits] property for BCD field", function() {
    var tf = function() { pxDocument = px.make([tools.missingDigitsBcd], pxFilePath); };
    expect(tf).toThrow("Property [digits] is not defined in object [#<Object>]");
  });

  it("px.open can open valid file", function() {
    pxDocument = px.make([tools.validTime], pxFilePath);
    pxDocument.close();
    pxDocument = px.open(pxFilePath);
    expect(pxDocument.fields[0].name).toEqual('timeField');
    expect(pxDocument.fields[0].type).toEqual('pxfTime');
  });

  it("px.open throws if no file name given", function() {
    expect(function () { px.open(); }).toThrow("Bad argument for px.open(path)");
  });

  it("px.open throws if given file does not exist", function() {
    expect(function () { px.open("does-not-exist"); })
      .toThrow("pxlib error [Could not open file of paradox database: No such file or directory] type [3]");
  });
});
