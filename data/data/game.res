;; game.xml
(pingus-resources
  (section (name "game")
    (sprite
      (name "loading")
      (image-file "../images/core/misc/loading.png"))

    (sprite
      (name "pingubw")
      (image-file "../images/core/misc/pingubw.png"))

    (sprite
      (name "404")
      (image-file "../images/core/misc/404.png"))

    (sprite
      (name "dot_green")
      (image-file "../images/core/worldmap/dot_green.png"))

    (sprite
      (name "dot_red")
      (image-file "../images/core/worldmap/dot_red.png"))

    (section (name "cursors")
      (sprite
        (name "editor")
        (image-file "../images/core/cursors/editor.png"))

      (sprite
        (name "cursor")
        (image-file "../images/core/cursors/cursor.png"))

      (sprite
        (name "scroll_cur")
        (image-file "../images/core/cursors/scroll.png"))

      (sprite
        (name "capgood")
        (origin "center")
        (offset 0 0)
        (image-file "../images/core/cursors/cap.png"))

      (sprite
        (name "capbad")
        (origin "center")
        (offset 0 0)
        (image-file "../images/core/cursors/capwrong.png"))

      (sprite
        (name "cross")
        (origin "center")
        (offset 0 0)
        (image-file "../images/core/cursors/cross.png"))

      (sprite
        (name "arrow_right")
        (origin "center")
        (offset 0 28)
        (image-file "../images/core/cursors/arrow_right.png"))

      (sprite
        (name "arrow_left")
        (origin "center")
        (offset 0 28)
        (image-file "../images/core/cursors/arrow_left.png"))
     )

    (section (name "smallmap")
      (sprite
        (name "entrance")
        (image-file "../images/core/misc/smallmap_entrance.png"))

      (sprite
        (name "exit")
        (image-file "../images/core/misc/smallmap_exit.png"))
     )

    (section (name "stars")
      (sprite
        (name "large_star")
        (image-file "../images/backgrounds/large_star.png"))

      (sprite
        (name "middle_star")
        (image-file "../images/backgrounds/middle_star.png"))

      (sprite
        (name "small_star")
        (image-file "../images/backgrounds/small_star.png"))

      (sprite
        (name "starfield_icon")
        (image-file "../images/backgrounds/starfield_icon.png"))
     )
   )
 )

;; EOF ;;
