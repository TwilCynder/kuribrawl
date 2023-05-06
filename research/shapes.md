Nécessité de pouvoir représenter plusieurs formes différentes, notament pour les hitboxes et les solid objects

## En java
On va utiliser le polymorphisme.  

Voir si la position fait partie des propriétés d'une shape.  

Si oui

```java
interface Shape {
  boolean isInside(int x, int y);
}

abstract class Area implements Shape {
  protected int x, y;
  
  ShapePosition(int x_, int y_){
    x = x_;
    y = y_;
  }
}

abstract class Rectangle extends Area {
  protected int w, h;
  
  Rectangle(int x, int y, int w_, int h_){
    super(x, y);
    w = w_;
    h = h-;
  }
  
  boolean isInside(int px, int py){
    return px > x && px < x + w && py > y && py < y + h;
  }
}
```

si non

```java
interface Shape {
  boolean isInside(int x, int y);
}

class Rectangle implements Shape {
  protected int w, h;
  
  Rectangle(int w_, int h_){
    w = w_;
    h = h_;
  }
  
  @Override
  boolean isInside(int x, int y){
    return x > 0 && x < w && y > 0 && y < h;
  }
  
  class Area {
    private Shape shape;
    private int x, y;
    
    Area(Shape shape_, int x_, int y_) {
      shape = shape_;
      x = x_;
      y = y_;
    }
    
    public boolean isInside(int px, int py){
      return shape.isInside(px - x; py - y);
    }
  }
}
```
