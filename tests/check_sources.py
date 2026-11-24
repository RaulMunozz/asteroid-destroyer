#!/usr/bin/env python3
"""Dependency-free source consistency check for the openFrameworks project.

Verifies that every file listed in CMakeLists.txt exists and that every quoted
local include resolves with exact case (important on case-sensitive filesystems).
Run from the repository root with: python tests/check_sources.py
"""
from pathlib import Path
import re
import sys

ROOT = (
    Path.cwd()
    if (Path.cwd() / "CMakeLists.txt").is_file()
    else Path(__file__).resolve().parent.parent
)
failures = []

# Headers provided by openFrameworks/core addons, not by this repository.
EXTERNAL_HEADERS = {
    "ofMain.h",
    "ofBaseApp.h",
    "ofEasyCam.h",
    "ofMath.h",
    "ofxAssimpModelLoader.h",
}

cmake = (ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
target = re.search(r"add_executable\s*\([^)]*\)", cmake, re.S)
if not target:
    failures.append("CMakeLists.txt: no add_executable block found")
else:
    for name in re.findall(r"[\w./+-]+\.(?:cpp|h|hpp|c)\b", target.group(0), re.I):
        if not (ROOT / name).is_file():
            failures.append(f"CMakeLists.txt: missing source {name}")

sources = list(ROOT.glob("*.cpp")) + list(ROOT.glob("*.h")) + list(ROOT.glob("*.hpp"))
for path in sources:
    text = path.read_text(encoding="utf-8", errors="replace")
    for include in re.findall(r'#\s*include\s*"([^"]+)"', text):
        if include in EXTERNAL_HEADERS:
            continue
        # Compare exact names so the check also works on case-insensitive filesystems.
        parent = (ROOT / include).parent
        if any(p.is_file() and p.name == Path(include).name for p in parent.glob("*")):
            continue
        matches = [p for p in ROOT.rglob("*") if p.is_file() and p.name.lower() == include.lower()]
        if matches:
            failures.append(
                f"{path.name}: include '{include}' has wrong case on disk "
                f"({matches[0].relative_to(ROOT).as_posix()})"
            )
        else:
            failures.append(f"{path.name}: include '{include}' not found")

for failure in failures:
    print(f"FAIL: {failure}")
print(f"\n{len(failures)} failure(s).")
sys.exit(1 if failures else 0)
