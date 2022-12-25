# Self-Adaptive Body Sensor Network (SA-BSN) Extension

This is the repository that contains the implementation of our group project of the course Fundamental of Adaptive Software. In this project, we extended the SA-BSN by considering an uncertainty of unpredictable power losses in the sensor circuitry. This is a fork of the original SA-BSN repository from which we worked upon. Here you can find [all the changes made from the last commit of the original repository until the HEAD of our edited repository](https://github.com/lkuffo/bsn/compare/1c45cd8f4c43e36fcf5665940d5ce7c66b907b31..HEAD?diff=unified).

Check our [demonstration video of the SA-BSN extension](). (TODO)

To compile and run the SA-BSN extension, follow the instructions (tested on macOS Monterey 12.1): 

## Detailed instructions to run and observe the result of SA-BSN extension
You do not need a specific OS to run the simulation. If you have all the needed software installed (Docker and Visual Studio Code), it will take around 10 minutes to set up and run. Follow these instructions:
### Run the project
- Install the latest version of [Docker Desktop](https://www.docker.com/products/docker-desktop/).
- Run Docker in your machine.
- Install the latest version of [Visual Studio Code](https://code.visualstudio.com/) (VSC)
- Clone this repository (https://github.com/lkuffo/bsn) using Git. If you are on Windows OS: Make sure Windows did not add the carriage return characters to every file of the repository. This will cause further steps to fail. You can easily identify if this is the case if all files are marked as *modified* by git.
- Open VSC.
- Go to VSC Extensions and install the extension called **"Dev Containers"**.
- Import the BSN repository folder in VSC.
- You will see a prompt in VSC requesting to open the repository inside of a Docker Dev Container. **Accept** it. This will reopen VSC and start the process of setting up the Docker container and building the BSN.
- After the build of BSN has finished, open a new terminal on VSC and run the following command: `bash ./run_tmux.sh 600`. Replace 600 with the time in seconds you wish to run the simulation. This will open 15 terminals (i.e. Data Access, Strategy Manager, Strategy Enactor, Logger, Probe, Effector, Central Hub (g4t1), Patient, 6 Sensors (g3t1_1 through g3t1_6) and Simulation (Injector)). If no terminal is opened instantly, you can wait until the simulation finishes and all the 15 terminals will suddenly appear with all the generated output. The latter is a bug of VSC.

### Observe the result
- At the end of the simulation you can check the logs of every component in the terminals. 
- In addition to this, you can check persisted logs by going to `src/sa-bsn/knowledge_repository/resource/logs`. To check the effects of the extension you should check the voltage status logs and the uncertainty logs. You will see how the voltage of sensors changes over time and how voltage uncertainties are injected. 
- By executing the Python script inside `src/sa-bsn/simulation/analyzer/voltageAnalyzer.py` you can plot the voltage of one sensor (G3_T1_5). You need to change the file name at the start of the script with the *voltagestatus* file name generated from the last executed simulation. You would also need to install the needed Python dependencies (matplotlib and numpy) since this is not part of the development environment.
