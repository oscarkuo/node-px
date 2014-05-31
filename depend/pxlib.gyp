{
  "targets": [
    {
      'target_name': 'px-0.6.5',
      'product_prefix': 'lib',
      'type': 'static_library',
      'sources': [
        'pxlib-0.6.5/src/gregor.c',
        'pxlib-0.6.5/src/px_crypt.c',
        'pxlib-0.6.5/src/px_error.c',
        'pxlib-0.6.5/src/px_io.c',
        'pxlib-0.6.5/src/px_memprof.c', 
        'pxlib-0.6.5/src/paradox.c',
        'pxlib-0.6.5/src/px_encode.c',
        'pxlib-0.6.5/src/px_head.c', 
        'pxlib-0.6.5/src/px_memory.c',
        'pxlib-0.6.5/src/px_misc.c'
      ],
      'include_dirs': [
        'pxlib-0.6.5/include'
      ]
    }
  ]
}
