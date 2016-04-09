# DisparityAlgorithm

This small service is responsible for the execution of different disparity algorithms.
It utilizes the following OpenCV disparity algorithms:

- StereoBM
- StereoSGBM

It also offers wrapper for two other suites.

### Usage

Adjust `local.ini`.

```bash
./DisparityAlgorithm <identifier> <algorithmId> <left> <right> <out>
```

