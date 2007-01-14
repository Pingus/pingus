;; worldobjs.xml
(pingus-resources
  (section (name "worldobjs")
    (sprite
      (name "switchdoor_box")
      (image (file "../images/worldobjs/switchdoor_box.png")))

    (sprite
      (name "switchdoor_tile")
      (image (file "../images/worldobjs/switchdoor_tile.png")))

    (sprite
      (name "switchdoor_switch")
      (image (file "../images/worldobjs/switchdoor_switch.png")))

    (sprite
      (name "switchdoor_tile_cmap")
      (image (file "../images/worldobjs/switchdoor_tile_cmap.png")))

    (sprite
      (name "teleporter2")
      (origin "center")
      (offset 0 0)
      (image (file "../images/worldobjs/teleporter2.png")))

    (sprite
      (name "infobox")
      (origin "bottom_center")
      (offset 0 0)
      (image (file "../images/worldobjs/infobox.png")))

    (sprite
      (name "teleporter")
      (loop #f)
      (speed 20)
      (origin "bottom_center")
      (offset 0 0)
      (image (file  "../images/worldobjs/teleporter.png")
             (array 6 1)
             (size 35 52)
             (pos 0 0)))

    (sprite
      (name "teleportertarget")
      (loop #f)
      (speed 15)
      (origin "center")
      (offset 0 0)
      (image (file  "../images/worldobjs/teleportertarget.png")
             (array 7 1)
             (size 64 64)
             (pos 0 0)))

    (sprite
      (name "conveyorbelt_left")
      (image (file  "../images/worldobjs/conveyorbelt_left.png")
             (array 15 1)
             (size 15 14)
             (pos 0 0)))

    (sprite
      (name "conveyorbelt_middle")
      (image (file  "../images/worldobjs/conveyorbelt_middle.png")
             (array 15 1)
             (size 15 14)
             (pos 0 0)))

    (sprite
      (name "conveyorbelt_right")
      (image (file  "../images/worldobjs/conveyorbelt_right.png")
             (array 15 1)
             (size 15 14)
             (pos 0 0)))

    (sprite
      (name "iceblock")
      (image (file  "../images/worldobjs/iceblock.png")
             (array 6 1)
             (size 25 10)
             (pos 0 0)))

    (sprite
      (name "iceblock_cmap")
      (image (file "../images/worldobjs/iceblock_cmap.png")))

    (sprite
      (name "conveyorbelt_cmap")
      (image (file "../images/worldobjs/conveyorbelt_cmap.png")))
   )
 )

;; EOF ;;
