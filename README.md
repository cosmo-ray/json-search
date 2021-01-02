small program that allow to search json object key.

example:

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
            "js/define/input.js",
            "js/define/img.js",
            "js/funtions/movement.js",
            "js/funtions/format.js",
            "js/funtions/colision.js",
            "js/funtions/actions.js",
            "js/funtions/Monster.js",
            "js/funtions/Player.js",
            "js/funtions/Bonus.js",
            "js/funtions/Bubble.js",
            "js/funtions/Effect.js",
            "js/funtions/Weapon.js",
            "js/funtions/affichage.js",
            "js/define/object.js",
            "js/game_level/patern02.js",
            "js/game_level/patern01.js",
            "js/game_level/lvl_01.js",
            "js/funtions/gameLoop.js",
        ]
    }
}

$ json-search  files .little-space/start.json
../little-space/start.json: [
  "js\/funtions\/Normes.js",
  "js\/define\/display.js",
  "js\/define\/input.js",
  "js\/define\/img.js",
  "js\/funtions\/movement.js",
  "js\/funtions\/format.js",
  "js\/funtions\/colision.js",
  "js\/funtions\/actions.js",
  "js\/funtions\/Monster.js",
  "js\/funtions\/Player.js",
  "js\/funtions\/Bonus.js",
  "js\/funtions\/Bubble.js",
  "js\/funtions\/Effect.js",
  "js\/funtions\/Weapon.js",
  "js\/funtions\/affichage.js",
  "js\/define\/object.js",
  "js\/game_level\/patern02.js",
  "js\/game_level\/patern01.js",
  "js\/game_level\/lvl_01.js",
  "js\/funtions\/gameLoop.js"
]
```
