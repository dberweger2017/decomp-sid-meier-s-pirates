# Local iPhoneOS 5.1 SDK

No iPhone is needed for this milestone. The SDK supplies target headers, frameworks, and libraries to the cross-compiler.

Apple documents that [Xcode 4.3.1 and 4.3.2 include iOS SDK 5.1](https://developer.apple.com/library/archive/documentation/Xcode/Conceptual/WhatsNewXcode-Archive/Articles/xcode_4_0.html). The game's metadata identifies Xcode 4.3.2 and SDK build 9B176, so start with that Xcode release. Apple's [Xcode resources](https://developer.apple.com/xcode/resources/) link to older releases in [More Downloads](https://developer.apple.com/download/all/).

A public third-party mirror has been pinned in `config/sdk-lock.json` to revision `f188cd4e635c290c8a041ce857981c3d1baebdc9`. Its SDK metadata reports version 5.1/build 9B176. This identifies the mirrored contents, not an authenticated Apple download.

```sh
python tools/sdk.py fetch
python configure.py --sdk "$PWD/build/sdk/iPhoneOS5.1.sdk"
python tools/dev.py doctor
python tools/validate_sdk.py
ninja
```

Fetch verifies the Git tree and full content fingerprint, preserves symlinks and keeps files in ignored build inputs. The mirror contains an original dangling CRFSuite library alias, recorded in SDK identity; object probes do not use it. Full-game linking remains unvalidated.

Alternatively, download Xcode 4.3.2 through Apple's download page, handling any sign-in yourself. Mount its disk image without installing or launching old Xcode, and copy this directory with `ditto` to preserve symlinks:

```text
Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk
```

Import that locally supplied SDK with:

```sh
python tools/sdk.py import --sdk /path/to/iPhoneOS5.1.sdk
```

The SDK inspector checks version, canonical name and build 9B176, then fingerprints file contents and symlink targets. SDK changes trigger Ninja rebuilds; stale SDK comparisons cannot verify. The historical container receives `/sdk` header paths, while editor settings receive local paths. GNU C++ 4.2.1 headers are selected explicitly for C++ and Objective-C++ units. Candidates still record their own defines, include paths and code-generation flags.

`tools/validate_sdk.py` compiles C/OpenGLES, C++ standard-library, Objective-C/Foundation and mixed Objective-C++ probes in ARM and Thumb modes, checking Mach-O symbols, header dependencies and repeated object hashes. It validates object compilation, not linking or every game candidate.

Hosted CI provisions the same pinned SDK for both source revisions and gates changes to the shared SDK baseline once candidates exist. A pre-provisioned runner path can use `PIRATES_SDK_PATH`; CI checks it against the pin. SDK files are excluded from Git, compiler images, caches and report uploads. No Apple account credentials are accessed.
