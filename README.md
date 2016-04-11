# disparity-evaluation

Small helpful programs for evaluating disparity algorithms.
It contains four different components:

- [1_DisparityAlgorithm](1_DisparityAlgorithm/)
- [2_BitmaskCreator](2_BitmaskCreator/)
- [3_DisparityEvaluation](3_DisparityEvaluation/)
- [4_NaiveImplementation](4_NaiveImplementation/)
- [4_NaiveImplementation](4_NaiveImplementation/)
- [5_ImageDiminisher](5_ImageDiminisher/)
- [6_WebViewer](6_WebViewer/)

### Prerequisites

Command Line Tools of Mac OS X:

```bash
xcode-select --install
```

Homebrew dependencies:

```bash
brew install cmake
brew install opencv3 --with-contrib --without-python
```

### How to run?

Build all the components with one script:

```bash
sh clean.sh
sh build.sh
```
