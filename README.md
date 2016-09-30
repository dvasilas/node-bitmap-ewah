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
* <a href="#push"><code><b>push()</b></code></a>
* <a href="#set"><code><b>set()</b></code></a>
* <a href="#unset"><code><b>unset()</b></code></a>
* <a href="#str"><code><b>toString()</b></code></a>
* <a href="#len"><code><b>length()</b></code></a>
* <a href="#ones"><code><b>numberOfOnes()</b></code></a>

<a name="ctor"></a>
#### createObject()
<code>createObject()</code> is the main entry point for creating a new compressed bitmap instance.

<a name="push"></a>
#### push(bit_pos)
<code>push()</code> appends a set bit (1) to the bitmap in position bit_pos. All bits between the last set bit and bit_pos are appended as unset bits (0).

<a name="set"></a>
#### set(bit_pos)
<code>set()</code> sets the bit in position bit_pos, provided it is already appended. Has no effect if bit is already set.

<a name="unset"></a>
#### unset(bit_pos)
<code>unset()</code> unsets the bit in position bit_pos, provided it is already appended. Has no effect if bit is already unset.

<a name="str"></a>
#### toString(delimeter)
<code>toString()</code> returns a string containing the positions of set bits, separated with the delimeter character.

<a name="len"></a>
#### length()
<code>length()</code> returns the uncopressed bitmap's size in bits.

<a name="ones"></a>
#### numberOfOnes()
<code>numberOfOnes()</code> returns the number of set bits in the bitmap.
