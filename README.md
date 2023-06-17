small program that allow to search json files.

# dependancies
```
json-c libc
```

# download:

on linux you have the appimages here: https://github.com/cosmo-ray/json-search/tags

# install:

Arch:
```
yay -S json-search-git
```

Macos:
```
brew tap cosmo-ray/oui
brew install cosmo-ray/oui/json-search
```

# usage:
```
json-search [OPTION] [NEEDLE] [FILES]
```
```
# options:
        -v: verbose mode
        -i: case insensitive
        -V: Check Value instead of keys
        -K: Print only objects keys
        -R: raw print
        -M NUM: limitate the number of returned objects
        -P: print parent instead of element
        -s: locate sub-string instead of strict comparison
                ex: 'file' will match with 'files'
        -r: recursive file search, NOT YET IMPLEMENTED
        -o: multiple patern search
        -t: look for object child, NOT YET IMPLEMENTED
        -l: location info: print file and key
        -h: are you really wondering what this is ?
```
But really just use
```sh
json-search -h
```
It's more updated than this readme

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

$ json-search  files little-space.json # "cat little-space.json | json-search files"  work too
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

# F.A.Q

Q: So it's just a cheap version of JQ

A: yes, like grep is a cheap version of sed, this tool try to be a simpler but easier to use version of jq that focus only on finding elements in json.
