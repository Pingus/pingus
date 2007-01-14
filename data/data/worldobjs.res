;; worldobjs.xml
(pingus-resources
  (section (name "worldobjs")
    (sprite
      (name "switchdoor_box")
      (image-file "../images/worldobjs/switchdoor_box.png"))

    (sprite
      (name "switchdoor_tile")
      (image-file "../images/worldobjs/switchdoor_tile.png"))

    (sprite
      (name "switchdoor_switch")
      (image-file "../images/worldobjs/switchdoor_switch.png"))

    (sprite
      (name "switchdoor_tile_cmap")
      (image-file "../images/worldobjs/switchdoor_tile_cmap.png"))

    (sprite
      (name "teleporter2")
      (origin "center")
      (offset 0 0)
      (image-file "../images/worldobjs/teleporter2.png"))

    (sprite
      (name "infobox")
      (origin "bottom_center")
      (offset 0 0)
      (image-file "../images/worldobjs/infobox.png"))

    (sprite
      (name "teleporter")
      (loop #f)
      (speed 20)
      (origin "bottom_center")
      (offset 0 0)
      (image-file "../images/worldobjs/teleporter.png")
      (image-array 6 1)
      (image-size 35 52)
      (image-pos 0 0))

    (sprite
      (name "teleportertarget")
      (loop #f)
      (speed 15)
      (origin "center")
      (offset 0 0)
      (image-file "../images/worldobjs/teleportertarget.png")
      (image-array 7 1)
      (image-size 64 64)
      (image-pos 0 0))

    (sprite
      (name "conveyorbelt_left")
      (image-file "../images/worldobjs/conveyorbelt_left.png")
      (image-array 15 1)
      (image-size 15 14)
      (image-pos 0 0))

    (sprite
      (name "conveyorbelt_middle")
      (image-file "../images/worldobjs/conveyorbelt_middle.png")
      (image-array 15 1)
      (image-size 15 14)
      (image-pos 0 0))

    (sprite
      (name "conveyorbelt_right")
      (image-file "../images/worldobjs/conveyorbelt_right.png")
      (image-array 15 1)
      (image-size 15 14)
      (image-pos 0 0))

    (sprite
      (name "iceblock")
      (image-file "../images/worldobjs/iceblock.png")
      (image-array 6 1)
      (image-size 25 10)
      (image-pos 0 0))

    (sprite
      (name "iceblock_cmap")
      (image-file "../images/worldobjs/iceblock_cmap.png"))

    (sprite
      (name "conveyorbelt_cmap")
      (image-file "../images/worldobjs/conveyorbelt_cmap.png"))
   )
 )

;; EOF ;;
