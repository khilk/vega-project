# raytracer
Tiny raytracer

**Требования:**


Необходимо реализовать рейтрейсер, с помощью которого можно
рендерить простейшие сцены

Сцена для рендеринга задаётся с помощью .obj файлов. Программа должна уметь рендерить такие файлы, а именно:
- Необходимо уметь обрабатывать строки с v, vn, f, mtllib и usemtl, другие игнорировать.
- В рамках данного проекта считать, что нумерация сущностей в f глобальная, т.е. группирующие модификаторы вроде g или o игнорируются.
- Необходимо уметь обрабатывать все возможные варианты задания вершин в f, т.е в том числе с индексом нормали и/или текстуры.
- Поддержка текстурирования в рамках данного проекта не предусмотрена.
- Для удобства сфера в рамках данного проекта будет задаваться такой строкой: S x y z r. Эта строка задает сферу с центром в (x, y, z)
радиуса r.
- Необходимо поддержать обработку строк вида P x y z r g b. Такая строка задает точечный источник света с координатами (x, y, z), имеющий
интенсивность (r, g, b).
- Необходимо уметь обрабатывать строки newmtl, Ka (поле ambient_color в Material), Kd (diffuse_color), Ks (specular_color), Ke (intensity), Ns (specular_exponent), Ni (refraction_index), остальное игнорировать.
- Необходимо уметь обрабатывать al a b c (albedo), задающий светоотражающие характеристики материала, по умолчанию значения должны быть al 1 0 0


Освещенность в точке $p$ некоторого объекта должна вычисляться по формуле: 
$I_{p} = I_{base}(p) + I_{comp}(p)$,
где $I_{base}​(p)=K_{a}​+K_{e}​+al_{0}​\sum_{m \in lights}​(K_{d} ​I_{d}​(p,m) + K_{s} ​I_{s}​(p,m))$

Сумма берется по всем источникам света, которые видны из точки $p$. $I_{d}$​ и $I_{s}$​  
это соответственно diffuse и specular составляющие освещения из модели Фонга

$I_{comp}​(p)=al_{1}​I_{reflect}​(p)+al_{2}​I_{refract}​(p)$

$I_{refract}(p)$ - освещенность от преломленного в точке $p$ луча, а $I_{reflect}(p)$ - освещенность
отраженного в $p$ луча


**Дополнительные источники:**

https://en.wikipedia.org/wiki/Phong_reflection_model

https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form

https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points.html

![alt text](https://github.com/khilk/raytracer/blob/main/test/models/deer/result.png)
