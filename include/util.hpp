// стандартный двумерный вектор 
struct vec2 {
    float x, y;
    vec2& operator+=(const vec2& v){ x += v.x; y += v.y; return *this;}
    vec2& operator*=(const float a){ x *= a; y *= a; return *this;}
    friend vec2 operator+(vec2 lhs, const vec2& rhs) { return lhs += rhs; }
    friend vec2 operator*(const float a, vec2 v) { v *= a; return v; }
};

// текстура из файла и ее размеры
struct Texture {
    int w, h;
    SDL_Texture *sdlTexture;
};
