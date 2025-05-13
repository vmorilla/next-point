Para resolver este problema de tiro parabólico, vamos a plantear las ecuaciones de movimiento y determinar las componentes de velocidad necesarias.
Consideremos:

El punto de lanzamiento está a una altura h
Queremos que toque tierra a una distancia horizontal d
Buscamos las componentes de velocidad inicial: vx y vy

Las ecuaciones del movimiento parabólico son:

Eje x: x = vx·t
Eje y: y = h + vy·t - (1/2)g·t²

Cuando el objeto toca tierra, y = 0 y x = d. Usando estas condiciones:
De la ecuación del eje x:
d = vx·t, por lo tanto t = d/vx
Sustituyendo este valor de t en la ecuación del eje y:
0 = h + vy·(d/vx) - (1/2)g·(d/vx)²
Despejando vy:
vy = (g·d²)/(2·vx²) - h·vx/d
Ahora bien, vx puede elegirse de manera arbitraria (siempre que sea distinto de cero). Una vez elegido vx, utilizamos la fórmula anterior para calcular vy.
Por tanto, las componentes de velocidad que necesitamos son:

vx: cualquier valor positivo (para moverse hacia la derecha)
vy = (g·d²)/(2·vx²) - h·vx/d

Esta solución tiene infinitas combinaciones posibles de vx y vy que lograrán que el proyectil toque tierra a la distancia d.

## Fijando la altura máxima

- vx = d.g/sqrt(2g(hn - h))
- vy = sqrt(2g(yn-h))

Ecuaciones:
- x = x0 + vx·t
- y = y0 + vy·t - (1/2)g·t²

En el punto máximo:
- dy = vy - g.t => en el punto máximo, dy = 0
- 0 = vy - g.t => tmax = vy / g

En el punto objetivo:
- 0 = y0 + vy.t2 - (1/2)g.t2²
- (1/2)g.t2² - vy.t2 - y0 = 0
- 
biquad => (-b +- sqrt(b2 - 4ac)) / 2a
- t2 = (+vy +- sqrt(vy² + 2g.y0)) / g
Solo una solución
- t2 = (vy + sqrt(vy² +2g.y0)) / g
  
## Fijando el ángulo de salida y la distancia, debo calcular v o algún componente de v
- Angulo salida = vy / vx :
    Cercano a 0: horizontal
    Cercano a -1: 45 grados arriba
    Cercano a +1: 45 grados abajo

- vy = ((1/2)g.t2² - y0)
- t2 = (vy + sqrt(vy² +2g.y0)) / g
- t2 = (x2 - x0) / vx




## Fijando altura en la red (para el saque?)

Sean x0 / y0 las coordenadas en el punto de salida
Sean x1 / y1 las coordenadas en el punto de la red
Y sean x2 / 0 las coordenadas en el punto objectivo

Ecuaciones:
- x = x0 + vx·t
- y = y0 + vy·t - (1/2)g·t²
 
Para x2:

    x2 = x0 + vx.t2
    0 = y0 + vy.t2 - (1/2)g.t² 
   
Para x1:

    x1 = x0 + vx.t1
    y1 = y0 + vy.t1 - (1/2)g.t1² 





En eje x tengo:

    (x1 - x0) = vx.t1  (distancia red)
    (x2 - x0) = vx.t2  (distancia objetivo)

    (x2 - x0) / (x1 - x0) = t2 / t1



En eje y tengo:

    (y1 - y0) = vy.t1 - (1/2)g.t1²
    -y0 = vy.t2 - (1/2)g.t2²

    ==

    vy.t1 + y0 - y1 = (1/2)g.t1²
    vy.t2 + y0 = (1/2)g.t2²

    ==

    vy.t2 + y0 = (1/2)g.t2²
    vy.t1 + y0 - y1 = (1/2)g.t1²

    ==

    (vy.t1 + y0 - y1) / (vy.t2 + y0) = (t1/t2)²

    [(vy.t2 + y0) - (y1 + vy.(t2 - t1))] / (vy.t2 + y0) = (t1/t2)²
    


Wolfram

solve {x = x0 + v1 * t, y = y0 + v2 - 1/2 * g * t^2, 0 = y0 + v2 - 1/2 * g * t2^2} for v1, v2


x≈-((0.5 i) g h)/sqrt(v^2 - g h) ∧ y≈(0.5 (2 v^2 - g h))/sqrt(v^2 - g h) ∧ d = -i h ∧ g h - v^2!=0 ∧ -i g h!=0