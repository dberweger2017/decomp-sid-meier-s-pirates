# Local iPhoneOS 5.1 SDK

No iPhone is needed for this milestone. The SDK supplies target headers, frameworks, and libraries to the cross-compiler.

Apple documents that [Xcode 4.3.1 and 4.3.2 include iOS SDK 5.1](https://developer.apple.com/library/archive/documentation/Xcode/Conceptual/WhatsNewXcode-Archive/Articles/xcode_4_0.html). The game's metadata identifies Xcode 4.3.2 and SDK build 9B176, so start with that Xcode release. Apple's [Xcode resources](https://developer.apple.com/xcode/resources/) link to older releases in [More Downloads](https://developer.apple.com/download/all/).

Download Xcode 4.3.2 yourself through Apple's download page. Any account sign-in happens in your own browser; this tooling does not access Apple credentials. Availability of that specific archive behind sign-in has not been verified here.

On macOS, mount the downloaded disk image and inspect `Xcode.app` without installing or launching it. Copy this directory out of the app bundle:

```text
Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk
```

A suitable ignored destination is `build/sdk/iPhoneOS5.1.sdk`. Preserve symlinks when copying, for example with `ditto`. Keep your current Xcode installation and developer directory selected.

After importing the original IPA and selecting a validated compiler:

```sh
python configure.py --sdk "$PWD/build/sdk/iPhoneOS5.1.sdk"
python tools/dev.py doctor
ninja
```

`doctor` checks the SDK version and canonical name in `SDKSettings.plist`. Actual compilation validates the headers needed by a candidate; a metadata check alone does not prove all SDK-dependent candidates work. Freestanding compiler probes use no SDK. SDK files are local inputs and are excluded from Git, compiler images, and report uploads.
