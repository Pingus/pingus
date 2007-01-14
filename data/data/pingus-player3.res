;; pingus-player3.xml
(pingus-resources
  (section (name "pingus")
    (section (name "player3")
      (section (name "rocketlauncher")
        (sprite
          (name "left")
          (loop #f)
          (speed 50)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/rocketlauncher.png")
                 (array 7 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (loop #f)
          (speed 50)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/rocketlauncher.png")
                 (array 7 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "boarder")
        (sprite
          (name "left")
          (image (file  "../images/pingus/player3/boarder.png")
                 (array 1 1)
                 (size 30 30)
                 (pos 0 0)))

        (sprite
          (name "right")
          (image (file  "../images/pingus/player3/boarder.png")
                 (array 1 1)
                 (size 30 30)
                 (pos 0 30)))
       )

      (sprite
        (name "angel")
        (image (file  "../images/pingus/player3/angel.png")
               (array 4 1)
               (size 46 30)
               (pos 0 0)))

      (sprite
        (name "superman")
        (image (file  "../images/pingus/player3/superman.png")
               (array 8 1)
               (size 30 30)
               (pos 0 0)))

      (section (name "bomber")
        (sprite
          (name "left")
          (loop #f)
          (speed 60)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/bomber.png")
                 (array 16 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (loop #f)
          (speed 60)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/bomber.png")
                 (array 16 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "basher")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 1)
          (image (file  "../images/pingus/player3/basher.png")
                 (array 12 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 1)
          (image (file  "../images/pingus/player3/basher.png")
                 (array 12 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "climber")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 16 2)
          (image (file  "../images/pingus/player3/climber.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset -14 2)
          (image (file  "../images/pingus/player3/climber.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "faller")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/faller.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/faller.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "jumper")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/faller.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/faller.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "tumbler")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/tumble.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/tumble.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "bridger")
        (sprite
          (name "left")
          (loop #f)
          (speed 66)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/bridger.png")
                 (array 15 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (loop #f)
          (speed 66)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/bridger.png")
                 (array 15 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "bridger_walk")
        (sprite
          (name "left")
          (loop #f)
          (speed 66)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/bridger_walk.png")
                 (array 4 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (loop #f)
          (speed 66)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/bridger_walk.png")
                 (array 4 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "walker")
        (sprite
          (name "left")
          (speed 60)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/walker.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (speed 60)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/walker.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "blocker")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/blocker.png")
                 (array 6 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/blocker.png")
                 (array 6 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "floater")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/floater.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/floater.png")
                 (array 8 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "digger")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/digger.png")
                 (array 14 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/digger.png")
                 (array 14 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "miner")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/miner.png")
                 (array 12 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/miner.png")
                 (array 12 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (sprite
        (name "splat")
        (origin "bottom_center")
        (offset 0 2)
        (loop #f)
        (speed 33)
        (image (file  "../images/pingus/player3/splat.png")
               (array 16 1)
               (size 32 32)
               (pos 0 0)))

      (section (name "exit")
        (sprite
          (name "left")
          (loop #f)
          (speed 60)
          (origin "bottom_center")
          (offset -2 2)
          (image (file  "../images/pingus/player3/exit.png")
                 (array 9 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (loop #f)
          (speed 60)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/exit.png")
                 (array 9 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "drownfall")
        (sprite
          (name "left")
          (loop #f)
          (speed 60)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/drownfall.png")
                 (array 15 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (loop #f)
          (speed 60)
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/drownfall.png")
                 (array 15 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "drownwalk")
        (sprite
          (name "left")
          (loop #f)
          (speed 60)
          (image (file  "../images/pingus/player3/drownwalk.png")
                 (array 15 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (loop #f)
          (speed 60)
          (image (file  "../images/pingus/player3/drownwalk.png")
                 (array 15 1)
                 (size 32 32)
                 (pos 0 32)))
       )

      (section (name "waiter")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/waiter.png")
                 (array 6 1)
                 (size 32 32)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/waiter.png")
                 (array 6 1)
                 (size 32 32)
                 (pos 0 0)))
       )

      (section (name "slider")
        (sprite
          (name "left")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/slider.png")
                 (array 1 1)
                 (size 32 15)
                 (pos 0 0)))

        (sprite
          (name "right")
          (origin "bottom_center")
          (offset 0 2)
          (image (file  "../images/pingus/player3/slider.png")
                 (array 1 1)
                 (size 32 15)
                 (pos 0 15)))
       )

      (sprite
        (name "explo")
        (image (file "../images/pingus/player3/explo.png")))
     )
   )
 )

;; EOF ;;
