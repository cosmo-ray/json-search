small program that allow to search json files.

# dependancies
```
json-c libc
```

# usage:
```
json-search NEEDLE FILE [FILES] [OPTION]
```
```
# options:
-v: verbose mode
-i: case insensitive
-s: locate sub-string instead of the strict comparison
-h: show help
... (plenty of other options)
```

# example:

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

# TODO:

* Handle whildcards
* Fix not implemented options
* add one option to add a maximum deep in search
* Enable to read json on stdin (ex: `cat file | json-search thing`)

# F.A.Q

Q: So it's just a cheap version of JQ

A: yes, like grep is a cheap version of sed, this tool try to be a simpler but easier to use version on jq that focus only on finding elements in json.
