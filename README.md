# node-bitmap-ewah
A set of NodeJS bindings for [a compressed bitmap data structure] (https://github.com/lemire/ewahboolarray).


### Installation
```sh
# We use node-gyp to build our binaries
npm install -g node-gyp

# Install node-bitmap-ewah
npm install node-bitmap-ewah
```

### Usage
```node
var Bitmap = require('node-bitmap-ewah');

var bitmap = Bitmap.createObject();
```

### API
* <a href="#ctor"><code><b>createObject()</b></code></a>

<a name="ctor"></a>
#### createObject()
<code>createObject()</code> is the main entry point for creating a new compressed bitmap instance.
