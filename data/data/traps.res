;; traps.xml
(pingus-resources
  (section (name "traps")
    (sprite
      (name "tut")
      (image-file "../images/traps/tut.png"))

    (sprite
      (name "smasher_cmap")
      (image-file "../images/traps/smasher_cmap.png"))

    (sprite
      (name "smasher")
      (image-file "../images/traps/smasher.png")
      (image-array 6 1)
      (image-size 282 189)
      (image-pos 0 0))

    (sprite
      (name "hammer")
      (loop #f)
      (speed 60)
      (image-file "../images/traps/hammer.png")
      (image-array 13 1)
      (image-size 151 181)
      (image-pos 0 0))

    (sprite
      (name "fake_exit")
      (origin "bottom_center")
      (offset 0 0)
      (image-file "../images/traps/fake_exit.png")
      (image-array 9 1)
      (image-size 75 112)
      (image-pos 0 0))

    (sprite
      (name "spike")
      (image-file "../images/traps/spike.png")
      (image-array 14 1)
      (image-size 32 32)
      (image-pos 0 0))

    (section (name "guillotinekill")
      (sprite
        (name "left")
        (image-file "../images/traps/guillotinekill.png")
        (image-array 12 1)
        (image-size 80 96)
        (image-pos 0 0))

      (sprite
        (name "right")
        (image-file "../images/traps/guillotinekill.png")
        (image-array 12 1)
        (image-size 80 96)
        (image-pos 0 96))
     )

    (sprite
      (name "guillotineidle")
      (image-file "../images/traps/guillotineidle.png")
      (image-array 7 1)
      (image-size 80 96)
      (image-pos 0 0))

    (sprite
      (name "laser_exit")
      (image-file "../images/traps/laser_exit.png")
      (image-array 6 1)
      (image-size 75 60)
      (image-pos 0 0))

    (sprite
      (name "bumper")
      (origin "bottom_center")
      (offset 0 0)
      (image-file "../images/traps/bumper.png")
      (image-array 6 1)
      (image-size 60 102)
      (image-pos 0 0))

    (sprite
      (name "bumper_cmap")
      (image-file "../images/traps/bumper_cmap.png"))
   )
 )

;; EOF ;;
