Structure MainMenuData
  cursorSprite.i
EndStructure
  
Declare startTestGame()  

Procedure menu_startGame()
  startTestGame()
EndProcedure

Procedure menu_quit()
  End
EndProcedure

Procedure startMenuRender(*menu.Menu, *data.MainMenuData)
  ClearScreen(#White)
  DisplaySprite(*data\cursorSprite, (#SCREEN_W / 2) - 160, *menu\selectedElement\position\y + 30)
  baseMenuRenderer(*menu, *data)
EndProcedure

Procedure initStartMenu()
  *menu.Menu = newMenu("start")
  *el1 = addMenuElement(*menu, (#SCREEN_W / 2) - 120, 100, #MENUCONTENT_IMAGE, getLoadedSprite("start_menu/start"), @menu_startGame())
  *el2 = addMenuElement(*menu, (#SCREEN_W / 2) - 120, 400, #MENUCONTENT_IMAGE, getLoadedSprite("start_menu/quit"), @menu_quit())
  
  *menu\render = @startMenuRender()
  
  *menu\defaultElement = *el1
  
  *data.MainMenuData = AllocateStructure(MainMenuData)
  *data\cursorSprite = getLoadedSprite("start_menu/cursor")
  *menu\data = *data
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 27
; Folding = -
; EnableXP