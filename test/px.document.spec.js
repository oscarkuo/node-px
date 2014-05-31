var px = require('../px');
var fs = require('fs');
var path = require('path');
var tools = require('./tools');

describe("px.document tests:", function () {
  var pxFilePath;

  beforeEach(function() {
    pxFilePath = tools.makePxFilePath('px.document');
    pxDocument = undefined;
  });

  afterEach(function() {
    tools.deletePxFile(pxDocument, pxFilePath);
  });

  it("fields returns all column types", function() {
    pxDocument = px.make(tools.validAllTypesInput, pxFilePath);
    expect(pxDocument.fields).toEqual(tools.validAllTypesOutput);
  });

  it("fields throws for closed px.document", function() {
    pxDocument = px.make([tools.validLong], pxFilePath);
    pxDocument.close();
    var tf = function () { pxDocument.fields };
    expect(tf).toThrow("document not opened");
  });

  it("count() returns 0 for empty database", function() {
    pxDocument = px.make(tools.validAllTypesInput, pxFilePath);
    expect(pxDocument.count()).toEqual(0);
  });
 
  it("put() throws when no argument provided", function() {
    pxDocument = px.make(tools.validAllTypesInput, pxFilePath);
    var tf = function() { pxDocument.put(); };
    expect(tf).toThrow("Bad argument for px.document.put(values)");
  });

  it("put() throws when argument is not an array", function() {
    pxDocument = px.make(tools.validAllTypesInput, pxFilePath);
    var tf = function() { pxDocument.put(999); };
    expect(tf).toThrow("[999] is not of type [array]");
  });

  it("put() throws when input array size don't match", function() {
    pxDocument = px.make(tools.validAllTypesInput, pxFilePath);
    var tf = function() { pxDocument.put([111, 222]); };
    expect(tf).toThrow(
      "[px.document.insert(values)] requires [10] fields but [2] provided");
  });

  var numericTestParams = [ 
    {
      fieldSpec : tools.validLong,
      textValue : 'xxx', 
      integerValue : undefined,
      decimalValue : 111.1, 
      type : 'pxfLong', 
      maxPlusOne : undefined,
      minMinusOne : undefined,
      validValue : 2147483647
    },
    {
      fieldSpec : tools.validShort,
      textValue : 'xxx', 
      integerValue : undefined,
      decimalValue : 111.1, 
      type : 'pxfShort',
      maxPlusOne : 32768,
      minMinusOne : -32769,
      validValue : 32767 
    },
    {
      fieldSpec : tools.validLogical,
      textValue : 'xxx', 
      integerValue : 1,
      decimalValue : 111.1, 
      type : 'pxfLogical', 
      maxPlusOne : undefined,
      minMinusOne : undefined,
      validValue : true
    },
    {
      fieldSpec : tools.validAlpha,
      textValue : undefined,
      integerValue : 1,
      decimalValue : 111.1, 
      type : 'pxfAlpha', 
      maxPlusOne : undefined,
      minMinusOne : undefined,
      oversizeData : 'xxxxxxxxxxxxxxxxxxxxx',
      validValue   : 'xxxxxxxxxxxxxxxxxxxx',
    },
    {
      fieldSpec : tools.validTimestamp,
      textValue : 'bad value',
      integerValue : 1,
      decimalValue : 111.1, 
      type : 'pxfTimestamp', 
      maxPlusOne : undefined,
      minMinusOne : undefined,
      oversizeData : undefined,
      validValue   : new Date(2000, 1, 1, 10, 50, 50, 0) // libpx cannot handle milliseconds
    },
    {
      fieldSpec : tools.validDate,
      textValue : 'bad value',
      integerValue : 1,
      decimalValue : 111.1, 
      type : 'pxfDate', 
      maxPlusOne : undefined,
      minMinusOne : undefined,
      oversizeData : undefined,
      validValue   : new Date(1971, 2, 4, 0, 0, 0, 0)
    },
  ];
  
  numericTestParams.forEach(function (p) {
    if (p.textValue != undefined) {
      it("put() throws when passing text for " + p.type, function() {
        pxDocument = px.make([p.fieldSpec], pxFilePath);
        var tf = function() { pxDocument.put([ p.textValue ]); };
        expect(tf).toThrow(
          "[" + p.textValue + "] is not of type [" + p.type + "]");
      });
    }

    if (p.decimalValue != undefined) {
      it("put() throws when passing decimal for " + p.type, function() {
        pxDocument = px.make([p.fieldSpec], pxFilePath);
        var tf = function() { pxDocument.put([ p.decimalValue ]); };
        expect(tf).toThrow(
          "[" + p.decimalValue + "] is not of type [" + p.type + "]");
      });
    }

    if (p.integerValue != undefined) {
      it("put() throws when passing integer for " + p.type, function() {
        pxDocument = px.make([p.fieldSpec], pxFilePath);
        var tf = function() { pxDocument.put([ p.integerValue]); };
        expect(tf).toThrow(
          "[" + p.integerValue + "] is not of type [" + p.type + "]");
      });
    }

    if (p.minMinusOne != undefined) {
      it("put() handles underflow for " + p.type, function() {
        pxDocument = px.make([p.fieldSpec], pxFilePath);
        var tf = function() { pxDocument.put([ p.minMinusOne ]); };
        expect(tf).toThrow(
          "[" + p.minMinusOne + "] is out of range for type [" + p.type + "]");
      });
    }

    if (p.oversizeData != undefined) {
      it("put() throws when data exceeds field size for " + p.type, function() {
        pxDocument = px.make([p.fieldSpec], pxFilePath);
        var tf = function() { pxDocument.put([ p.oversizeData]); };
        expect(tf).toThrow(
          "[" + p.oversizeData + "] is out of range for type [" + p.type +  
          "(" + p.fieldSpec.size + ")]");
      });
    }

    if (p.maxPlusOne != undefined) {
      it("put() handles overflow for " + p.type, function() {
        pxDocument = px.make([p.fieldSpec], pxFilePath);
        var tf = function() { pxDocument.put([ p.maxPlusOne]); };
        expect(tf).toThrow(
          "[" + p.maxPlusOne + "] is out of range for type [" + p.type + "]");
      });
    }

    it("put() works for valid " + p.type + " value", function() {
      pxDocument = px.make([p.fieldSpec], pxFilePath);
      pxDocument.put([ p.validValue ]);
      expect(pxDocument.count()).toEqual(1);
    });

    it("get() throws when passing non-integer parameter", function() {
      pxDocument = px.make([p.fieldSpec], pxFilePath);
      pxDocument.put([ p.validValue ]);
      var tf = function() { pxDocument.get('aaa'); };
      expect(tf).toThrow("[aaa] is not of type [integer]");
    });

    it("get() throws when passing non-integer parameter", function() {
      pxDocument = px.make([p.fieldSpec], pxFilePath);
      pxDocument.put([ p.validValue ]);
      var tf = function() { pxDocument.get(); };
      expect(tf).toThrow("Bad argument for px.document.get(index)");
    });

    it("get() works for valid " + p.type + " value", function() {
      pxDocument = px.make([p.fieldSpec], pxFilePath);
      pxDocument.put([ p.validValue ]);
      expect(pxDocument.get(0)[p.fieldSpec.name]).toEqual(p.validValue);
    });
  }); 
});
