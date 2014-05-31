node-px
=======
I started working on this node module because I wanted to use node.js and node-webkit for a project that requires using Paradox DB. It is based on pxlib (see http://pxlib.sf.net) but I wanted to expose a more JavaScript friendly API.


# Status
This is in the very early stage of development, API changes frequently and may eat you cat (if you have one).

# Known limitations

* No support for index
* Only supports pxfAlpha, pxfShort, pxfLong, pxfLogical, pxfTimestamp and pxfDate at the moment.
* No async API.
* Only lightly tested on few different linux distros and very briefly in Windows (won't even compile out of box).
* Conversion between JavaScript Date and Paradox Date/Time/Timestamp is buggy (mainly due to the fact that pxlib don't support millisecond even though both Paradox and JavaScript do).

# Build instructions
* Checkout the project from github
* Execute `node-gyp configure` and then `node-gyp build` from the project root directly will build and then execute unit-tests.
* Under Windows you'll need to install libiconv library and development files from GNU-Win32 project and tweak the binding.gyp files.
