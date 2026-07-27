#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include <cstring>
#include <discord_rpc.h>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const std::string fragShader = R"(
#version 120
uniform vec2 resolution;
uniform float time;
uniform int colorMode;
uniform vec3 accentColor;

vec2 getTarget(float idx) {
    float r = fract(sin(idx * 12.9898) * 43758.5453);
    if (r < 0.333) return vec2(0.0, 0.0);
    if (r < 0.666) return vec2(0.5, 0.0);
    return vec2(0.25, 0.4330127);
}

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.y;
    uv -= vec2(0.5 * resolution.x / resolution.y, 0.5);
    
    float angle = sin(time * 0.4) * 0.6 + cos(time * 0.15) * 0.4;
    float s = sin(angle), c = cos(angle);
    uv = mat2(c, -s, s, c) * uv; 
    
    float speed = 0.5; 
    float cycle_float = time * speed;
    float cycle = fract(cycle_float); 
    float idx = floor(cycle_float);   
    
    float pan_progress = smoothstep(0.0, 1.0, cycle);
    vec2 target = getTarget(idx);
    float zoom = exp2(-cycle);
    
    vec2 C = vec2(0.5, 0.2886751);
    vec2 p = uv * zoom + target * pan_progress + C * zoom;
    
    float sqrt3 = 1.7320508;
    vec2 st;
    st.y = p.y * 2.0 / sqrt3;
    st.x = p.x - p.y / sqrt3;
    
    bool isHole = false;
    for(int i = -5; i < 15; i++) {
        vec2 grid = floor(st * exp2(float(i)));
        if (mod(abs(grid.x), 2.0) >= 1.0 && mod(abs(grid.y), 2.0) >= 1.0) {
            isHole = true;
            break;
        }
    }
    
    if (isHole) {
        gl_FragColor = vec4(0.02, 0.0, 0.05, 1.0); 
    } else {
        if (colorMode == 0) {
            float r = 0.5 + 0.5 * sin(uv.x * 10.0 + time);
            float g = 0.5 + 0.5 * sin(uv.y * 10.0 - time);
            float b = 0.7 + 0.3 * cos(time * 1.5);
            gl_FragColor = vec4(r, g, b, 1.0);
        } else {
            float intensity = 0.6 + 0.4 * cos(time * 2.0 + uv.x * 5.0);
            gl_FragColor = vec4(accentColor * intensity, 1.0);
        }
    }
}
)";

struct ColorBtn {
    sf::FloatRect rect;
    sf::Color uiColor;
    int mode; 
    sf::Vector3f shaderCol;
};

struct Slider {
    sf::FloatRect bounds;
    float value; // от 0.0 до 1.0
    std::string name;
};

// Функции сохранения и загрузки настроек в settings.mlpeg
void saveSettings(int mode, const sf::Vector3f& col) {
    std::ofstream file("settings.mlpeg");
    if (file.is_open()) {
        file << mode << "\n" << col.x << "\n" << col.y << "\n" << col.z << "\n";
    }
}

void loadSettings(int& mode, sf::Vector3f& col, std::vector<Slider>& sliders) {
    std::ifstream file("settings.mlpeg");
    if (file.is_open()) {
        file >> mode >> col.x >> col.y >> col.z;
        sliders[0].value = col.x;
        sliders[1].value = col.y;
        sliders[2].value = col.z;
    }
}

int main() {
    DiscordEventHandlers handlers;
    std::memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize("1531399571161219132", &handlers, 1, nullptr);

    DiscordRichPresence discordPresence;
    std::memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Looking at Serpinsky triangles";
    Discord_UpdatePresence(&discordPresence);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "STriangle", sf::State::Fullscreen);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    sf::Image icon;
    if (icon.loadFromFile("icon.png")) {
        window.setIcon(icon.getSize(), icon.getPixelsPtr());
    }

    sf::Vector2u winSize = window.getSize();
    sf::RectangleShape screenRect(sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y)));
    
    if (!sf::Shader::isAvailable()) {
        std::cerr << "Шейдеры не поддерживаются!" << std::endl;
        return -1;
    }

    sf::Shader shader;
    if (!shader.loadFromMemory(fragShader, sf::Shader::Type::Fragment)) {
        std::cerr << "Ошибка компиляции шейдера!" << std::endl;
        return -1;
    }

    bool showSettings = false;
    int colorMode = 0; 
    sf::Vector3f accentColor = {1.0f, 1.0f, 1.0f};

    float centerX = winSize.x / 2.0f;
    float centerY = winSize.y / 2.0f;

    // Кнопка сброса на Радугу (по центру сверху)
    ColorBtn rainbowBtn = { { {centerX - 100.0f, centerY - 180.0f}, {200.0f, 50.0f} }, sf::Color(100, 100, 100), 0, {0,0,0} };

    // Слайдеры для ручной настройки своего цвета (Red, Green, Blue)
    std::vector<Slider> sliders = {
        { { {centerX - 150.0f, centerY - 90.0f}, {300.0f, 30.0f} }, 1.0f, "Red" },
        { { {centerX - 150.0f, centerY - 30.0f}, {300.0f, 30.0f} }, 1.0f, "Green" },
        { { {centerX - 150.0f, centerY + 30.0f}, {300.0f, 30.0f} }, 1.0f, "Blue" }
    };

    // Загружаем сохраненные настройки из settings.mlpeg, если файл существует
    loadSettings(colorMode, accentColor, sliders);

    sf::Clock clock;

    while (window.isOpen()) {
        Discord_RunCallbacks();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                if (key->code == sf::Keyboard::Key::B && key->shift) {
                    showSettings = !showSettings;
                    window.setMouseCursorVisible(showSettings);
                }
            }
            else if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (showSettings && mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
                    
                    // Клик по кнопке Радуги
                    if (rainbowBtn.rect.contains(mousePos)) {
                        colorMode = 0;
                        saveSettings(colorMode, accentColor);
                    }
                }
            }
        }

        // Если настройки открыты и зажата ЛКМ — двигаем слайдеры
        if (showSettings && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2i mPos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            for (auto& slider : sliders) {
                if (slider.bounds.contains(mousePos)) {
                    float val = (mousePos.x - slider.bounds.position.x) / slider.bounds.size.x;
                    if (val < 0.0f) val = 0.0f;
                    if (val > 1.0f) val = 1.0f;
                    slider.value = val;

                    colorMode = 1; // Автоматически переключаем на кастомный цвет
                    accentColor = {sliders[0].value, sliders[1].value, sliders[2].value};
                    saveSettings(colorMode, accentColor);
                }
            }
        }

        shader.setUniform("resolution", sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y)));
        shader.setUniform("time", static_cast<float>(clock.getElapsedTime().asSeconds()));
        shader.setUniform("colorMode", colorMode);
        shader.setUniform("accentColor", accentColor);

        window.clear();
        
        sf::RenderStates states;
        states.shader = &shader;
        window.draw(screenRect, states);

        // Отрисовка графического меню настроек при нажатии Shift + B
        if (showSettings) {
            sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(winSize.x), static_cast<float>(winSize.y)));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            // Кнопка Радуга
            sf::RectangleShape rbRect(rainbowBtn.rect.size);
            rbRect.setPosition(rainbowBtn.rect.position);
            rbRect.setFillColor(rainbowBtn.uiColor);
            if (colorMode == 0) {
                rbRect.setOutlineThickness(3);
                rbRect.setOutlineColor(sf::Color::White);
            }
            window.draw(rbRect);

            // Отрисовка слайдеров RGB
            for (const auto& slider : sliders) {
                // Фон слайдера
                sf::RectangleShape track(slider.bounds.size);
                track.setPosition(slider.bounds.position);
                track.setFillColor(sf::Color(50, 50, 50));
                window.draw(track);

                // Заполненная часть слайдера
                sf::RectangleShape fill(sf::Vector2f(slider.bounds.size.x * slider.value, slider.bounds.size.y));
                fill.setPosition(slider.bounds.position);
                if (slider.name == "Red") fill.setFillColor(sf::Color(255, 50, 50));
                else if (slider.name == "Green") fill.setFillColor(sf::Color(50, 255, 50));
                else if (slider.name == "Blue") fill.setFillColor(sf::Color(50, 50, 255));
                window.draw(fill);
            }

            // Прямоугольник-предпросмотр выбранного кастомного цвета
            sf::RectangleShape preview(sf::Vector2f(300.0f, 50.0f));
            preview.setPosition(sf::Vector2f(centerX - 150.0f, centerY + 90.0f));
            preview.setFillColor(sf::Color(
                static_cast<std::uint8_t>(accentColor.x * 255),
                static_cast<std::uint8_t>(accentColor.y * 255),
                static_cast<std::uint8_t>(accentColor.z * 255)
            ));
            if (colorMode == 1) {
                preview.setOutlineThickness(3);
                preview.setOutlineColor(sf::Color::White);
            }
            window.draw(preview);
        }

        window.display();
    }

    Discord_Shutdown();
    return 0;
}