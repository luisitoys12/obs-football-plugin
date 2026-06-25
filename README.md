# ⚽ OBS Football Plugin

> Plugin nativo para OBS Studio con overlays estilo broadcast profesional — Liga MX, ESPN, FOX Sports.

![License](https://img.shields.io/badge/license-GPL--2.0-blue) ![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey) ![OBS](https://img.shields.io/badge/OBS-30%2B-purple)

---

## 📺 Características

### Scorebug (Bug del Marcador)
- Marcador compacto estilo ESPN/FOX en esquina configurable
- Logos de equipos con escudo real (vía API-Football)
- Cronómetro en vivo con tiempo de descuento
- Indicador de periodo (1T / 2T / ET / PEN)
- Animación de gol con flash y expansión

### Lower Thirds Broadcast
- Entrada de jugador con stats (goles, asistencias, minuto)
- Tarjeta amarilla/roja con efecto barrido
- Sustituciones con flechas animadas
- Alineaciones en slide-in
- Efecto GOOOL animado

### Panel Dock en OBS (Qt6)
- Integrado como dock nativo dentro de OBS
- Botones grandes para eventos en vivo
- Selector de partido desde API-Football
- Timer con controles

### Integración API-Football
- Partidos en vivo (Liga MX, Champions, Copa del Mundo, etc.)
- Sincronización automática de marcador
- Alineaciones, estadísticas y eventos
- Auto-refresh configurable
- Modo manual para partidos locales

### Temas Visuales
- 🇲🇽 **Liga MX** (tema principal)
- ESPN Classic
- FOX Sports
- Copa América

---

## 🏗️ Compilación

### Requisitos
- CMake 3.20+
- Qt6 (6.4+)
- OBS Studio 30+ (con headers de desarrollo)
- libcurl
- nlohmann/json

### Windows
```bash
cmake -B build -G "Visual Studio 17 2022" -A x64 \
  -DCMAKE_PREFIX_PATH="C:/OBS/cmake"
cmake --build build --config Release
```

### Linux
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

---

## ⚙️ Instalación

1. Copia `obs-football-plugin.dll` a `%APPDATA%\obs-studio\plugins\obs-football\bin\64bit\`
2. Copia la carpeta `data/obs-plugins/obs-football/` a `%APPDATA%\obs-studio\plugins\obs-football\data\`
3. Reinicia OBS Studio
4. En OBS: **Herramientas → Football Plugin → Configurar API Key**

---

## 🔑 API-Football

Obtén tu API key gratis en [api-sports.io](https://api-sports.io) y configúrala en:
`OBS → Herramientas → Football Plugin Settings`

---

## 📁 Estructura del Proyecto

```
obs-football-plugin/
├── src/
│   ├── plugin-main.cpp
│   ├── football-source.cpp/hpp
│   ├── scoreboard.cpp/hpp
│   ├── timer.cpp/hpp
│   ├── api-football.cpp/hpp
│   ├── graphics/
│   │   ├── renderer.cpp
│   │   ├── scorebug.cpp
│   │   ├── lower-third.cpp
│   │   └── transitions.cpp
│   └── ui/
│       ├── control-dock.cpp/hpp
│       ├── team-editor.cpp
│       └── settings-dialog.cpp
├── data/obs-plugins/obs-football/
│   ├── locale/
│   │   ├── en-US.ini
│   │   └── es-ES.ini
│   └── shaders/
├── assets/
│   ├── fonts/
│   └── textures/
├── cmake/
├── CMakeLists.txt
└── buildspec.json
```

---

## 📜 Licencia

GPL-2.0 — Compatible con OBS Studio.

---

## 👤 Autor

**Luis Martinez Sandoval** — [Estacion Kusmedios](https://estacionkusmedios.org)  
Desarrollo impulsado por [Antigravity](https://github.com/antigravity) + Perplexity AI
