# disparity-evaluation

Small helpful programs for evaluating disparity algorithms.
It contains four different components:

- [1_DisparityAlgorithm](1_DisparityAlgorithm/)
- [2_BitmaskCreator](2_BitmaskCreator/)
- [3_DisparityEvaluation](3_DisparityEvaluation/)

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

### Evaluate with

```bash
python eval-chain.py {algorithm} {dataset} {sequence}
```

For instance:

```bash
python eval-chain.py OpenCVSGBM SVDDD 01_bunny
```
