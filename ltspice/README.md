# LTspice commands

## Shortcuts

In this [document](https://www.analog.com/media/en/news-marketing-collateral/solutions-bulletins-brochures/ltspice-keyboard-shortcuts.pdf?modelType=spice-models) you will find all possible LTspice shortcuts and directives.

## Editing shortcuts

* **F2**: component list
* **F3**: wiring components | circuit
* **F4**: label net and ground
* **F5**: REMOVE
* **F6**: DUPLICATE
* **F7**: MOVE
* **Ctrl+R**: ROTATE
* **Spice directives**: RIGHT clicking -> DRAFT -> Spice directive
* **Comments**: RIGHT clicking -> DRAFT -> Comment
* [source](https://www.eecg.utoronto.ca/~ali/spice/LTspice/LTSpiceShortGuide.pdf)

## Transient Analysis

* directive format: `.tran <stop_time>`
* Example: `.tran 5ms`
* We'll need to define a waveform using the *SINE* directive
  * `SINE(<Voffset>, <Vamp>, <frequency> <time_delay> <theta_1/s> <phi_deq> <n_cycles>)`
  * [source](https://spiceman.net/ltspice-transient-analysis/)

## AC Analysis (magnitude - phase)

* directive format: `.ac <type_of_sweep> <number_of_points_per_octave> <start_frequency> <stop_frequency>`
* Example: `.ac oct 1000 1 48k`

## Running an Analysis

* Click RUN button, the first in the top left menu.
  * New window should be opened
* With the multimeter lead icon, select the wiring point you want to meter voltage
  * For meter current, just `click+SHIFT`

## Exporting Data Analysis

1. In the analysis window, click the CONTROL PANEL button (fourth button in the top left menu).
2. In WAVEFORMS, click the Data Export Tool button.
3. This opens a new window with all measures you have analyzed.
   1. Select the ones you need - for instance, V(vout) or V(n002) in the examples.
   2. Define the output file path to store the TXT file.
   3. Click OK button
4. It should generates a TXT file that includes
   1. *timestamps* and the *voltage* in the transient analysis
   2. *frequency* and *magnitude & phase* values in AC analysis
