small program that allow to search json files.

#dependancies
```
json-c libc
```

#usage:
```
json-search NEEDLE FILE [FILES] [OPTION]
```
```
#options:
-v: verbose mode
-i: case insensitive
-s: locate sub-string instead of the strict comparison
-h: show help
... (plenty of other options)
```

#example:

```
$cat little-space.json:
{
    "name" : "little-space",
    "pre-load" : [
        { "file" : "html5-emu.js", "type" : "js" },
    ],
    "init-scripts" : [
        ["js", "js_emu_init"]
    ],
    "make-prototype": "jumper",
    "starting widget" : "little-space",
    "little-space" : {
        "<type>": "html-emu",
        "dmod": "little-space",
        "files": [
            "js/funtions/Normes.js",
            "js/define/display.js",
            "js/define/input.js"
        ]
    }
}

$ json-search  files .little-space/start.json
../little-space/start.json: [
      "js/funtions/Normes.js",
      "js/define/display.js",
      "js/define/input.js"
]
```
