#pragma once

namespace yunu3DAPI
{
    struct Matrix4x4
    {
        float m11 = 1, m12 = 0, m13 = 0, m14 = 0;
        float m21 = 0, m22 = 1, m23 = 0, m24 = 0;
        float m31 = 0, m32 = 0, m33 = 1, m34 = 0;
        float m41 = 0, m42 = 0, m43 = 0, m44 = 1;
    };
}
