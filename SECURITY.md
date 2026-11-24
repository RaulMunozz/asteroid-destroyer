# Security Policy

**Last reviewed:** 2026-11-24  
**Scope:** `RaulMunozz/asteroid-destroyer` — a local 3D arcade game built with C++20 and openFrameworks.

## Architecture

- Desktop executable, not a web service.
- Keyboard input only (`main.cpp`, `ofApp.cpp`).
- No HTTP server, sockets, APIs, database, cloud storage or authentication.
- No user accounts, sessions, cookies or forms.
- No secrets, tokens, `.env` files or credentials in the repository.
- Build: `CMakeLists.txt` (C++20). openFrameworks 0.12.1 is documented in `README.md` but is not pinned by a lockfile or CI.
- External assets (FBX models, WAV/MP3 audio) are read from fixed relative paths and are not committed to the repository.

## Applicability of OWASP controls

OWASP Top 10 web categories such as SQL injection, XSS, CSRF, SSRF, broken access control, IDOR, open redirects, insecure deserialization, server-side template injection, HTTP parameter pollution and mass assignment do not apply: the project has no web layer, server, user input over a network or persistent data.

Authentication, session management, MFA, password hashing, rate limiting and secure cookies do not apply because there are no users or accounts.

## Implemented protections

| Protection | Implementation |
|---|---|
| Portability / build integrity | `ofApp.h` includes `asteroid.h` with the exact filename case, so the project also builds on case-sensitive filesystems. |
| Asset load diagnostics | `ofApp.cpp` and `asteroid.h` check the result of `loadModel()` and emit `ofLogWarning` when a model is missing or invalid. Audio loads remain non-fatal, matching the original behaviour. |
| Fixed asset paths | Models and sounds are loaded only from hard-coded relative paths; there is no user-controlled path, so path traversal is not possible. |
| No dynamic code | No `eval`, `system`, shell execution, deserialization or plugin loading in project code. |
| Secrets | No credentials or API keys are present. |

## Known limitations

- **Low — third-party parser trust:** FBX and audio files are parsed by openFrameworks/Assimp. A malicious local asset could crash the process. An attacker would already need local write access. Only use trusted assets.
- **Low — supply chain:** openFrameworks and its transitive native libraries are not version-pinned in `CMakeLists.txt`; only `README.md` documents 0.12.1. There is no CI or dependency lockfile.
- **Low — error handling:** asset failures are warnings, not fatal errors. The game can run with missing visuals/audio. This preserves the original behaviour.
- **Informational — functional gap:** `README.md` documents an `R` reset key that is not implemented in `keyPressed()`. This is a documentation/behaviour mismatch, not a security issue.

## Secure build and deployment

1. Use openFrameworks 0.12.1 as documented.
2. Keep model and audio files in trusted, read-only locations relative to the working directory.
3. Build locally with `cmake` and your openFrameworks toolchain; do not expose the executable as a network service.
4. Review this file when changing asset paths, adding file formats or adding any network/input feature.

## Verification performed

- Remote source reviewed through the GitHub API; local copy matched all blobs.
- Searches for network calls, SQL, secrets, `eval`/`system`, dynamic loading and form/API code returned no matches.
- No build was executed here: `cmake`, `cl`, `msbuild`, `clang++` and an openFrameworks installation are not available in this environment.

## Maintenance

- Re-run the build on Windows and, if possible, on a case-sensitive filesystem after changes.
- Keep the openFrameworks version documented in `README.md` aligned with the actual build environment.
- If networking, file uploads or user-controlled paths are ever added, perform a new audit covering input validation, authentication and transport security.
