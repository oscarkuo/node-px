{
  "targets": [
    {
      'target_name' : 'run-tests',
      'type' : 'none',
      'dependencies' : ['copy-module'],
      'actions' : [
        { 
          'variables' : { 'test_specs' : [ 'test' ] },
          'action_name' : 'jasmine-tests',
          'inputs' : [''],
          'outputs' : [''] ,
          'action' : [ 'jasmine-node', '<@(test_specs)' ],
        }
      ]
    },

    {
      'target_name' : 'copy-module',
      'type' : 'none',
      'dependencies' : [ 'px' ],
      'copies' : [ { 'destination' : '.', 'files' : [ '<(PRODUCT_DIR)/px.node' ] } ]
    },

    {
      'target_name': 'px',
      'defines': [ 'BUILD_NODE_EXTENSION' ],
      'sources': [ 
        "src/node.cc", "src/paradox.cc", "src/errors.cc",
        "src/document.cc", "src/global.cc", "src/npx.cc" 
      ],
      'cflags': [ '-Wall' ],
      'cflags!' : [ '-fno-exceptions'],
      'cflags_cc!': [ '-fno-exceptions' ],
      'include_dirs': [ 'include', 'depend/pxlib-0.6.5/include' ],
      'dependencies': [ 'depend/pxlib.gyp:px-0.6.5' ],
    },
  ]
}
