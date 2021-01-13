Enumeration
  #MENUCONTENT_IMAGE
  #MENUCONTENT_TEXT
  #MENUCONTENT_IDK_MAYBE_BOTH_OR_SOMETHING_ENTIRELY_DIFFERENT
EndEnumeration  
  
Prototype.i menuEffect(*data)
Prototype.i menuRenderer(*menu, *menuData)
Prototype.i menuElementRenderer(*element, *menu)

Structure MenuElement
  *effect.menuEffect
  *nextelement.MenuElement[4]
  position.Vector
  contentType.b
  StructureUnion
    *contentText.String
    contentImage.i
    *content
  EndStructureUnion
  *renderer.menuElementRenderer
EndStructure

Structure Menu
  List elements.MenuElement()
  *data
  *render.menuRenderer  ;replaces the basic renderer
  *onDraw.menuRenderer  ;is called after the basic renderer
  needRedraw.b
EndStructure  

Procedure newMenu(name.s)
  *menu.Menu = AllocateStructure(Menu)
  
EndProcedure

Procedure renderMenu(*menu.Menu)
  Define *element.MenuElement
  If *menu\needRedraw
    ForEach *menu\elements()
      *element = @*menu\elements()
      
      If *element\renderer
        *element\renderer(*element, *menu)
      ElseIf *element\contentType = #MENUCONTENT_IMAGE
        DisplaySprite(*element\contentImage, *element\position\x, *element\position\y)
      ElseIf *element\contentType = #MENUCONTENT_TEXT
      Else
      EndIf 
    Next
  EndIf 
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 8
; Folding = -
; EnableXP