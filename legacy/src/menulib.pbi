Enumeration
  #MENUCONTENT_IMAGE
  #MENUCONTENT_TEXT
  #MENUCONTENT_OTHER
EndEnumeration  
  
Prototype.i menuEffect(*data)
Prototype.i menuCallback(*menu, *menuData)
Prototype.i menuElementCallback(*element, *menu)

Structure MenuElement
  id.l
  *effect.menuEffect
  *nextelement.MenuElement[4]
  position.Vector
  contentType.b
  StructureUnion
    *contentText.String
    contentImage.i
    *content
  EndStructureUnion
  *renderer.menuElementCallback
  *start.menuCallback
EndStructure

Structure Menu
  List elements.MenuElement()
  *data
  *render.menuCallback  ;replaces the basic renderer
  *init.menuCallback
  needRedraw.b
  *selectedElement.MenuElement
  *defaultElement.MenuElement
EndStructure  

Procedure baseMenuRenderer(*menu.Menu, *data)
  Define *element.MenuElement
  If *menu\needRedraw
    ForEach *menu\elements()
      *element = @*menu\elements()
      If *element\renderer
        *element\renderer(*element, *menu)
      ElseIf *element\contentType = #MENUCONTENT_IMAGE And *element\contentImage
        DisplaySprite(*element\contentImage, *element\position\x, *element\position\y)
      ElseIf *element\contentType = #MENUCONTENT_TEXT
      Else
      EndIf 
    Next
  EndIf 
EndProcedure

Procedure renderMenu(*menu.Menu)
  If *menu\render
    *menu\render(*menu, *menu\data)
  EndIf 
  FlipBuffers()
EndProcedure

Procedure addMenuElement(*menu.Menu, x.l, y.l, contentType.b = #MENUCONTENT_OTHER, content.i = 0, *effect = 0, *renderer = 0)
  *element.MenuElement = AddElement(*menu\elements())
  *element\position\x = x
  *element\position\y = y
  *element\contentType = contentType
  *element\content = content
  *element\effect = *effect
  *element\renderer = *renderer
  *menu\selectedElement = *element
  
  ProcedureReturn *element
EndProcedure

Procedure setMenuElementRelation(*element1.MenuElement, *element2.MenuElement, direction.b, reciprocity.b = 1)
  *element1\nextelement[direction] = *element2
  If reciprocity
    *element2\nextelement[(direction + 2) % 4] = *element1
  EndIf 
EndProcedure

Procedure menuOnInput(type.b, element.b)
EndProcedure
; IDE Options = PureBasic 5.72 (Windows - x64)
; CursorPosition = 78
; FirstLine = 28
; Folding = -
; EnableXP