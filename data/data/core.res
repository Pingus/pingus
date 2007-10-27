;; -*- scheme -*-
;; core.xml
(pingus-resources
 (section (name "levelsets") ;; doesn't really belong here 
                   (sprite
                    (name "halloween")
                    (image-file "../images/levelsets/halloween.png"))
           )

 (section (name "core")
          (section (name "cursors")
                   (sprite
                    (name "editor")
                    (image-file "../images/core/cursors/editor.png"))

                   (sprite
                    (name "cursor")
                    (image-file "../images/core/cursors/cursor.png"))

                   (sprite
                    (name "scroll_cur")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/cursors/scroll.png"))

                   (sprite
                    (name "capgood")
                    (image-file "../images/core/cursors/cap.png"))

                   (sprite
                    (name "capbad")
                    (image-file "../images/core/cursors/capwrong.png"))

                   (sprite
                    (name "cross")
                    (image-file "../images/core/cursors/cross.png"))

                   (sprite
                    (name "arrow_right")
                    (image-file "../images/core/cursors/arrow_right.png"))

                   (sprite
                    (name "arrow_left")
                    (image-file "../images/core/cursors/arrow_left.png"))

                   (sprite
                    (name "animcross")
                    (image-file "../images/core/cursors/animcross.png")
                    (offset -18 -18)
                    (image-array 10 1))
                   )

          (section (name "editor")
                   (sprite
                    (name "solidcolorbackground_thumb")
                    (image-file "../images/core/editor/solidcolorbackground_thumb.png"))

                   (sprite
                    (name "solidcolorbackground")
                    (image-file "../images/core/editor/solidcolorbackground.png"))

                   (sprite
                    (name "starfield_thumb")
                    (image-file "../images/core/editor/starfield_thumb.png"))

                   (sprite
                    (name "starfield")
                    (image-file "../images/core/editor/starfield.png"))

                   (sprite
                    (name "combobox")
                    (image-file "../images/core/editor/combobox.png"))

                   (sprite
                    (name "obj_background")
                    (image-file "../images/core/editor/obj_background.png"))

                   (sprite
                    (name "obj_button")
                    (image-file "../images/core/editor/obj_button.png"))

                   (sprite
                    (name "obj_button-pressed")
                    (image-file "../images/core/editor/obj_button-pressed.png"))

                   (sprite
                    (name "obj_button-raised")
                    (image-file "../images/core/editor/obj_button-raised.png"))

                   (sprite
                    (name "obj_exit")
                    (image-file "../images/core/editor/obj_exit.png"))

                   (sprite
                    (name "obj_entrance")
                    (image-file "../images/core/editor/obj_entrance.png"))

                   (sprite
                    (name "obj_gp_bridge")
                    (image-file "../images/core/editor/obj_gp_bridge.png"))

                   (sprite
                    (name "obj_gp_solid")
                    (image-file "../images/core/editor/obj_gp_solid.png"))

                   (sprite
                    (name "obj_gp_ground")
                    (image-file "../images/core/editor/obj_gp_ground.png"))

                   (sprite
                    (name "obj_gp_remove")
                    (image-file "../images/core/editor/obj_gp_remove.png"))

                   (sprite
                    (name "obj_gp_transparent")
                    (image-file "../images/core/editor/obj_gp_transparent.png"))

                   (sprite
                    (name "obj_hotspot")
                    (image-file "../images/core/editor/obj_hotspot.png"))

                   (sprite
                    (name "obj_liquid")
                    (image-file "../images/core/editor/obj_liquid.png"))

                   (sprite
                    (name "obj_trap")
                    (image-file "../images/core/editor/obj_trap.png"))

                   (sprite
                    (name "obj_weather")
                    (image-file "../images/core/editor/obj_weather.png"))

                   (sprite
                    (name "obj_worldobj")
                    (image-file "../images/core/editor/obj_worldobj.png"))




                   (sprite
                    (name "object-bottom")
                    (image-file "../images/core/editor/object-bottom.png"))

                   (sprite
                    (name "object-top")
                    (image-file "../images/core/editor/object-top.png"))


                   (sprite
                    (name "document-properties")
                    (image-file "../images/core/editor/document-properties.png"))

                   (sprite
                    (name "objects")
                    (image-file "../images/core/editor/objects.png"))

                   (sprite
                    (name "minimap")
                    (image-file "../images/core/editor/minimap.png"))

                   (sprite
                    (name "object-properties")
                    (image-file "../images/core/editor/object-properties.png"))

                   (sprite
                    (name "snap-grid")
                    (image-file "../images/core/editor/snap-grid.png"))

                   (sprite
                    (name "object-up")
                    (image-file "../images/core/editor/object-up.png"))

                   (sprite
                    (name "object-down")
                    (image-file "../images/core/editor/object-down.png"))

                   (sprite
                    (name "object-flip-horizontal")
                    (image-file "../images/core/editor/object-flip_horizontal.png"))

                   (sprite
                    (name "play")
                    (image-file "../images/core/editor/play.png"))

                   (sprite
                    (name "object-rotate-right")
                    (image-file "../images/core/editor/object-rotate_right.png"))

                   (sprite
                    (name "object-rotate-left")
                    (image-file "../images/core/editor/object-rotate_left.png"))

                   (sprite
                    (name "object-flip-vertical")
                    (image-file "../images/core/editor/object-flip_vertical.png"))

                   (sprite
                    (name "document-new")
                    (image-file "../images/core/editor/document-new.png"))

                   (sprite
                    (name "document-open")
                    (image-file "../images/core/editor/document-open.png"))

                   (sprite
                    (name "document-save")
                    (image-file "../images/core/editor/document-save.png"))

                   (sprite
                    (name "document-save-as")
                    (image-file "../images/core/editor/document-save-as.png"))

                   (sprite
                    (name "separator")
                    (image-file "../images/core/editor/separator.png"))

                   (sprite
                    (name "start_pos")
                    (image-file "../images/core/editor/start_pos.png"))

                   (sprite
                    (name "weather_rain")
                    (image-file "../images/core/editor/weather_rain.png"))

                   (sprite
                    (name "weather_snow")
                    (image-file "../images/core/editor/weather_snow.png"))

                   (sprite
                    (name "copy")
                    (image-file "../images/core/editor/edit-copy.png"))

                   (sprite
                    (name "new")
                    (image-file "../images/core/editor/tb_new.png"))

                   (sprite
                    (name "open")
                    (image-file "../images/core/editor/tb_open.png"))

                   (sprite
                    (name "save")
                    (image-file "../images/core/editor/tb_save.png"))

                   (sprite
                    (name "exit")
                    (image-file "../images/core/editor/tb_exit.png"))

                   (sprite
                    (name "clock_run")
                    (image-file "../images/core/editor/tb_clock_run.png"))

                   (sprite
                    (name "edit")
                    (image-file "../images/core/editor/tb_edit.png"))

                   (sprite
                    (name "preferences")
                    (image-file "../images/core/editor/tb_preferences.png"))

                   (sprite
                    (name "help")
                    (image-file "../images/core/editor/help.png"))

                   (sprite
                    (name "delete")
                    (image-file "../images/core/editor/edit-delete.png"))

                   (sprite
                    (name "index")
                    (image-file "../images/core/editor/tb_index.png"))

                   (sprite
                    (name "region-zoom")
                    (image-file "../images/core/editor/region-zoom.png"))

                   (sprite
                    (name "zoom-in")
                    (image-file "../images/core/editor/zoom-in.png"))

                   (sprite
                    (name "zoom-out")
                    (image-file "../images/core/editor/zoom-out.png"))

                   (sprite
                    (name "button")
                    (image-file "../images/core/editor/button.png"))

                   (sprite
                    (name "button-raised")
                    (image-file "../images/core/editor/button-raised.png"))

                   (sprite
                    (name "button-pressed")
                    (image-file "../images/core/editor/button-pressed.png"))

                   (sprite
                    (name "button_pressed")
                    (image-file "../images/core/editor/button_pressed.png"))

                   (sprite
                    (name "actions")
                    (image-file "../images/core/editor/actions.png"))

                   (sprite
                    (name "logo")
                    (image-file "../images/core/editor/logo.png"))
                   )

          (section (name "buttons")
                   (sprite
                    (name "info")
                    (origin "top_center")
                    (offset 0 0)
                    (image-file "../images/core/buttons/info.png"))

                   (sprite
                    (name "hbuttonbg")
                    (image-file "../images/core/buttons/hbuttonbg.png"))

                   (sprite
                    (name "hbuttonbgb")
                    (image-file "../images/core/buttons/hbuttonbgb.png"))

                   (sprite
                    (name "buttonbackground")
                    (image-file "../images/core/buttons/buttonbackground.png"))

                   (sprite
                    (name "buttonbackgroundhl")
                    (image-file "../images/core/buttons/buttonbackgroundhl.png"))

                   (sprite
                    (name "pause")
                    (image-file "../images/core/buttons/pause.png"))

                   (sprite
                    (name "fast_forward")
                    (image-file "../images/core/buttons/fast_forward.png"))

                   (sprite
                    (name "armageddon_anim")
                    (image-file "../images/core/buttons/armageddon_anim.png")
                    (image-array 14 1)
                    (image-size 36 57)
                    (image-pos 0 0))
                   )

          (section (name "menu")
                   (sprite
                    (name "menuitem")
                    (origin "center")
                    (image-file "../images/core/menu/menuitem.png"))

                   (sprite
                    (name "menuitem_highlight")
                    (origin "center")
                    (image-file "../images/core/menu/menuitem_highlight.png"))

                   (sprite
                    (name "marker")
                    (image-file "../images/core/menu/marker.png"))

                   (sprite
                    (name "marker_locked")
                    (image-file "../images/core/menu/marker_locked.png"))

                   (sprite
                    (name "marker2")
                    (image-file "../images/core/menu/marker2.png"))

                   (sprite
                    (name "filedialog")
                    (image-file "../images/core/menu/filedialog.png"))

                   (sprite
                    (name "optionmenu")
                    (image-file "../images/core/menu/optionmenu.png"))

                   (sprite
                    (name "folder")
                    (image-file "../images/core/menu/folder.png"))

                   (sprite
                    (name "parent_folder")
                    (image-file "../images/core/menu/parent_folder.png"))

                   (sprite
                    (name "default_level")
                    (image-file "../images/core/menu/default_level.jpg"))

                   (sprite
                    (name "up_arrow")
                    (image-file "../images/core/menu/up_arrow.png"))

                   (sprite
                    (name "down_arrow")
                    (image-file "../images/core/menu/down_arrow.png"))

                   (sprite
                    (name "startscreenbg")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/menu/startscreenbg.jpg"))

                   (sprite
                    (name "create_on")
                    (image-file "../images/core/menu/create_on.png"))

                   (sprite
                    (name "credits_on")
                    (image-file "../images/core/menu/credits_on.png"))

                   (sprite
                    (name "exit_on")
                    (image-file "../images/core/menu/exit_on.png"))

                   (sprite
                    (name "options_on")
                    (image-file "../images/core/menu/options_on.png"))

                   (sprite
                    (name "play_on")
                    (image-file "../images/core/menu/play_on.png"))

                   (sprite
                    (name "multi_on")
                    (image-file "../images/core/menu/multi_on.png"))

                   (sprite
                    (name "layer1")
                    (image-file "../images/core/menu/layer1.png"))

                   (sprite
                    (name "layer2")
                    (image-file "../images/core/menu/layer2.png"))

                   (sprite
                    (name "layer3")
                    (image-file "../images/core/menu/layer3.png"))

                   (sprite
                    (name "layer4")
                    (image-file "../images/core/menu/layer4.png"))

                   (sprite
                    (name "layer5")
                    (image-file "../images/core/menu/layer5.png"))

                   (sprite
                    (name "exit_button_normal")
                    (image-file "../images/core/menu/exit_button_normal.png"))

                   (sprite
                    (name "exit_button_hover")
                    (image-file "../images/core/menu/exit_button_hover.png"))

                   (sprite
                    (name "exit_button_pressed")
                    (image-file "../images/core/menu/exit_button_pressed.png"))

                   (sprite
                    (name "exit_menu")
                    (image-file "../images/core/menu/exit_menu.png"))
                   )

          (section (name "start")
                   (sprite
                    (name "ok")
                    (image-file "../images/core/misc/start_ok.png"))

                   (sprite
                    (name "ok_hover")
                    (image-file "../images/core/misc/start_ok_hover.png"))

                   (sprite
                    (name "ok_clicked")
                    (image-file "../images/core/misc/start_ok_clicked.png"))

                   (sprite
                    (name "back")
                    (image-file "../images/core/misc/start_back.png"))

                   (sprite
                    (name "back_hover")
                    (image-file "../images/core/misc/start_back_hover.png"))

                   (sprite
                    (name "back_clicked")
                    (image-file "../images/core/misc/start_back_clicked.png"))
                   )

          (section (name "misc")
                   (sprite
                    (name "checkbox_clicked")
                    (image-file "../images/core/misc/checkbox_clicked.png"))

                   (sprite
                    (name "next")
                    (image-file "../images/core/misc/next.png"))

                   (sprite
                    (name "next_hover")
                    (image-file "../images/core/misc/next_hover.png"))

                   (sprite
                    (name "chalk_pingu1")
                    (image-file "../images/core/misc/chalk_pingu1.png"))

                   (sprite
                    (name "chalk_pingu2")
                    (image-file "../images/core/misc/chalk_pingu2.png"))

                   (sprite
                    (name "chalk_pingu3")
                    (image-file "../images/core/misc/chalk_pingu3.png"))

                   (sprite
                    (name "chalk_pingu4")
                    (image-file "../images/core/misc/chalk_pingu4.png"))

                   (sprite
                    (name "flag0")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/flag0.png"))

                   (sprite
                    (name "404")
                    (image-file "../images/core/misc/404.png"))

                   (sprite
                    (name "404sprite")
                    (image-file "../images/core/misc/404.png"))

                   (sprite
                    (name "logo")
                    (image-file "../images/core/misc/logo.png"))

                   (sprite
                    (name "loading")
                    (image-file "../images/core/misc/loading.png"))

                   (sprite
                    (name "pingubw")
                    (image-file "../images/core/misc/pingubw.png"))

                   (sprite
                    (name "creditpingu")
                    (origin "center")
                    (image-file "../images/core/misc/creditpingu.png"))

                   (sprite
                    (name "smallmap_entrance")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/smallmap_entrance.png"))

                   (sprite
                    (name "smallmap_exit")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/smallmap_exit.png"))

                   (sprite
                    (name "flag0")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/flag0.png"))

                   (sprite
                    (name "flag1")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/flag1.png"))

                   (sprite
                    (name "flag2")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/flag2.png"))

                   (sprite
                    (name "flag3")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/flag3.png"))

                   (sprite
                    (name "unplayable")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/misc/unplayable.png"))

                   (sprite
                    (name "unplayable2")
                    (image-file "../images/core/misc/unplayable2.png"))

                   (sprite
                    (name "infinity")
                    (image-file "../images/core/misc/infinity.png"))
                   )

          (section (name "worldmap")
                   (section (name "pingus")
                            (sprite
                             (name "left")
                             (speed 60)
                             (origin "bottom_center")
                             (offset 0 2)
                             (image-file "../images/core/worldmap/pingus.png")
                             (image-array 8 1)
                             (image-size 24 24)
                             (image-pos 0 0))

                            (sprite
                             (name "right")
                             (speed 60)
                             (origin "bottom_center")
                             (offset 0 2)
                             (image-file "../images/core/worldmap/pingus.png")
                             (image-array 8 1)
                             (image-size 24 24)
                             (image-pos 0 24))
                            )

                   (sprite
                    (name "tube")
                    (image-file "../images/core/worldmap/tube.png"))

                   (sprite
                    (name "dot_invalid")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/dot_invalid.png"))

                   (sprite
                    (name "dot_green")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/dot_green.png"))

                   (sprite
                    (name "dot_red")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/dot_red.png"))

                   (sprite
                    (name "dot_green_hl")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/dot_green_hl.png"))

                   (sprite
                    (name "dot_red_hl")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/dot_red_hl.png"))

                   (sprite
                    (name "flaggreen")
                    (origin "center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/flaggreen.png"))

                   (sprite
                    (name "arrow")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/arrow.png"))

                   (sprite
                    (name "pingus_standing")
                    (origin "bottom_center")
                    (offset 0 0)
                    (image-file "../images/core/worldmap/pingus_standing.png"))

                   (sprite
                    (name "levelname_bg")
                    (image-file "../images/core/worldmap/levelname_bg.png"))

                   (sprite
                    (name "enter_button_normal")
                    (image-file "../images/core/worldmap/enter_button_normal.png"))

                   (sprite
                    (name "enter_button_hover")
                    (image-file "../images/core/worldmap/enter_button_hover.png"))

                   (sprite
                    (name "enter_button_pressed")
                    (image-file "../images/core/worldmap/enter_button_pressed.png"))

                   (sprite
                    (name "leave_button_normal")
                    (image-file "../images/core/worldmap/leave_button_normal.png"))

                   (sprite
                    (name "leave_button_hover")
                    (image-file "../images/core/worldmap/leave_button_hover.png"))

                   (sprite
                    (name "leave_button_pressed")
                    (image-file "../images/core/worldmap/leave_button_pressed.png"))

                   (sprite
                    (name "story_button_normal")
                    (image-file "../images/core/worldmap/story_button_normal.png"))

                   (sprite
                    (name "story_button_hover")
                    (image-file "../images/core/worldmap/story_button_hover.png"))

                   (sprite
                    (name "story_button_pressed")
                    (image-file "../images/core/worldmap/story_button_pressed.png"))

                   (sprite
                    (name "credits_button_normal")
                    (image-file "../images/core/worldmap/credits_button_normal.png"))

                   (sprite
                    (name "credits_button_hover")
                    (image-file "../images/core/worldmap/credits_button_hover.png"))

                   (sprite
                    (name "credits_button_pressed")
                    (image-file "../images/core/worldmap/credits_button_pressed.png"))
                   )
          )
 )

;; EOF ;;
