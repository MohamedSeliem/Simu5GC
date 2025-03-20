# Simu5GC - 5G Core Simulation in OMNeT++

![Simu5GC](https://img.shields.io/badge/OMNeT%2B%2B-5G--Core-blue) ![Work in Progress](https://img.shields.io/badge/Status-Work%20in%20Progress-orange)

Simu5GC is a **5G Core (5GC) simulation** project built using **OMNeT++ and Simu5G**. It models the key components of a 5G Core network, supporting essential control and user plane functionalities, enabling research on **network slicing, QoS, traffic management, and TSN integration**.

## Features
- Implementation of **5GC functions** (AMF, SMF, AUSF, UDM, UDR, PCF, BSF, UPF, NSSF).
- **Network slicing** simulation with real slice selection logic.
- **5G-TSN integration** for industrial and deterministic networking scenarios.
- Implements **N3, N4, N6, N7, N8, N10, N11, N13, N15, N18, N19, and N21** interfaces.
- Built on **Simu5G and INET framework** in OMNeT++.
- Open5GS-inspired architecture.

## Installation
### Prerequisites
Ensure you have the following installed:
- [OMNeT++](https://omnetpp.org/download.html) (Recommended: 6.0+)
- [INET Framework](https://inet.omnetpp.org/) (Latest stable version)
- [Simu5G](https://github.com/Unipisa/Simu5G) (5G simulation module for OMNeT++)

### Clone the Repository
```bash
git clone https://github.com/MohamedSeliem/Simu5GC.git
cd Simu5GC
```

### Build the Project
1. Open OMNeT++ IDE.
2. Import the project (`File` > `Import` > `Existing Projects into Workspace`).
3. Right-click the project and select **Build Project**.

## Usage
1. Configure the **ini file** for different scenarios.
2. Run the simulation in OMNeT++.
3. Analyze results using **scalar and vector files**.

## Project Structure
```
Simu5GC/
├── src/           # Source code for 5G Core functions
├── simulations/   # Example simulation configurations
├── ini/           # OMNeT++ configuration files
├── results/       # Output logs, vectors, and scalars
├── README.md      # This file
└── .gitignore     # Ignore unnecessary files
```

## Future Work
- Enhancing **slicing and mobility** in simulations.
- Improving **5G-TSN integration**.
- Implementing **NRF and real-world policies** in PCF.

## Contribution
Contributions are welcome! Feel free to open issues or submit pull requests.

## License
This project is licensed under the **MIT License**.

## Contact
Maintainer: **Mohamed Seliem**  
GitHub: [@MohamedSeliem](https://github.com/MohamedSeliem)

