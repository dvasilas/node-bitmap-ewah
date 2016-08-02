{
    "targets": [
    {
        "target_name": "node-bitmap-ewah",
        "sources": [ "bitmap_ewah.cc", "EWAHBitMap.cc" ],
        "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "<!(node -e \"require('bindings')\")",
            "./src/EWAHBoolArray/headers/"
        ]
    }
  ]
}
