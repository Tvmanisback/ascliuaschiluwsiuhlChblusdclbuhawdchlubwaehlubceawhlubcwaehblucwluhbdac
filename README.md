# Ascendant

RPG with Gorilla Tag-style arm locomotion (C++ → WASM) + PlayFab + Photon (Realtime + Chat App / Voice).

## IDs (already configured)

- **PlayFab Title ID**: `18B3AB`
- **Photon Realtime App ID**: `d5e92ab9-8ac5-4660-b9a9-c67e63fd6d38`
- **Photon Chat App ID** (Voice): `93e399f8-e31a-4b5b-9f99-6f25e19bf53e`

Photon Custom Authentication URL (set this on both Photon apps):

```
https://18B3AB.playfabapi.com/photon/authenticate
```

## Project structure

```
Ascendant/
├── cpp/
│   ├── gorilla_movement.h
│   └── gorilla_movement.cpp      ← movement library
├── js/
│   ├── config.js                 ← all IDs
│   ├── playfab.js
│   └── photon.js
├── public/
│   └── index.html
├── build.yml                     ← separate CI / APK-oriented workflow
└── README.md
```

## Build the C++ movement library (WASM)

You need [Emscripten](https://emscripten.org/).

```bash
cd cpp
emcc gorilla_movement.cpp -O2 -s WASM=1 -s EXPORTED_FUNCTIONS="['_gorilla_init','_gorilla_set_hands','_gorilla_set_gripping','_gorilla_update','_gorilla_get_position','_gorilla_get_velocity','_gorilla_reset']" -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","_malloc","_free"]' -o ../public/gorilla_movement.js
```

This produces `gorilla_movement.js` + `gorilla_movement.wasm` in `public/`.

## Run locally

```bash
npx serve public
```

Open the URL it prints.

## PlayFab → Photon flow

1. Login with PlayFab (`LoginWithCustomID` or any other login).
2. Call `GetPhotonAuthenticationToken` once for Realtime App ID.
3. Call `GetPhotonAuthenticationToken` once for Chat App ID (Voice).
4. Connect Photon clients using **Custom Authentication**:
   - `username` = PlayFabId
   - `token` = the token returned by PlayFab

See `js/photon.js` for the exact shape.

## Notes

- Voice is intentionally exposed as **Chat App** in the code (`photonChatAppId`).
- Full Photon JS SDK must still be added (official Photon client scripts).
- This is a browser / WebXR-ready base. A native Android APK requires a separate engine (Unity / Godot / native NDK) and is not generated here.
