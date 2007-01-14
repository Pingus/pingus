;; pingus-common.xml
(pingus-resources
  (section (name "other")
    (sprite
      (name "brick_right")
      (image-file "../images/pingus/common/brick_right.png"))

    (sprite
      (name "brick_left")
      (image-file "../images/pingus/common/brick_left.png"))

    (sprite
      (name "bash_radius")
      (image-file "../images/pingus/common/bash_radius.png"))

    (sprite
      (name "bash_radius_gfx")
      (image-file "../images/pingus/common/bash_radius_gfx.png"))

    (sprite
      (name "digger_radius")
      (image-file "../images/pingus/common/digger_radius.png"))

    (sprite
      (name "digger_radius_gfx")
      (image-file "../images/pingus/common/digger_radius_gfx.png"))

    (sprite
      (name "bomber_radius")
      (image-file "../images/pingus/common/bomber_radius.png"))

    (sprite
      (name "bomber_radius_gfx")
      (image-file "../images/pingus/common/bomber_radius_gfx.png"))

    (sprite
      (name "light")
      (image-file "../images/pingus/common/light.png")
      (origin "center")
      (offset 0 0))

    (sprite
      (name "splash")
      (image-file "../images/pingus/common/splash.png")
      (image-array 12 1)
      (image-size 32 32)
      (image-pos 0 0))

    (sprite
      (name "splash_debris")
      (image-file "../images/pingus/common/splash_debris.png")
      (image-array 6 2)
      (image-size 32 32)
      (image-pos 0 0))

    (section (name "laser_kill")
      (sprite
        (name "left")
        (loop #f)
        (speed 60)
        (origin "bottom_center")
        (offset 0 2)
        (image-file "../images/pingus/common/laser_kill.png")
        (image-array 9 1)
        (image-size 33 32)
        (image-pos 0 0))

      (sprite
        (name "right")
        (loop #f)
        (speed 60)
        (origin "bottom_center")
        (offset 0 2)
        (image-file "../images/pingus/common/laser_kill.png")
        (image-array 9 1)
        (image-size 33 32)
        (image-pos 0 32))
     )

    (section (name "floaterlayer")
      (sprite
        (name "left")
        (origin "bottom_center")
        (offset 0 2)
        (image-file "../images/pingus/common/floaterlayer.png")
        (image-array 8 1)
        (image-size 32 32)
        (image-pos 0 0))

      (sprite
        (name "right")
        (origin "bottom_center")
        (offset 0 2)
        (image-file "../images/pingus/common/floaterlayer.png")
        (image-array 8 1)
        (image-size 32 32)
        (image-pos 0 32))
     )
   )

  (section (name "XMas")
    (sprite
      (name "walker")
      (image-file "../images/pingus/common/xmas-walker.png")
      (image-array 8 2)
      (image-size 32 44)
      (image-pos 0 0))
   )
 )

;; EOF ;;
