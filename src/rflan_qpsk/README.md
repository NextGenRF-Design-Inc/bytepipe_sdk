![logo](../../docs/BytePipe_Logo.png)

# RFLAN QPSK Example

The example follows these steps:
1. Navigate to QPSK simulink file and simulate
2. Generate HDL
3. Simulate TX and RX blocks in Vivado
4. Generate IP
5. Integrate TX and RX blocks into basic bytepipe project and simulate (excluding interaction with ADRV9002)
6. Integrate TX and RX into basic bytepipe project and perform live test (including interaction with ADRV9002 - basic loopback configuration)

### 1. Navigate to QPSK simulink file and simulate
### 2. Generate HDL
### 3. Simulate TX and RX blocks in Vivado
### 4. Generate IP
### 5. Integrate TX and RX blocks into basic bytepipe project and simulate (excluding interaction with ADRV9002)
### 6. Integrate TX and RX into basic bytepipe project and perform live test (including interaction with ADRV9002 - basic loopback configuration)

<!--The rflan qpsk example utilizes the basic [rflan build](https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/src/rflan/README.md) to send baseband information over a bandpass channel. 

The example follows these steps:
1. Generate input bits
2. Transform symbols into I and Q samples 
3. Write samples to bytepipe using the CLI (command line interface)
4. Bytepipe streams samples to ADRV9002 RF IC for modulation
5. Bandpass signal interacts with communication channel
6. Bytepipe streams demodulated samples from ADRV9002 RF IC 
7. Read samples from bytepipe using the CLI (command line interface)
8. transform I and Q samples into output bits
9. Compare received and transmitted bits

### 1.Generate input bits

We chose to randomly generate packet payloads of 2240 bits. 
Each payload is accompanied by a 26 bit preamble in each packet.
Frame Efficiency = (payload_length)/(payload_length + overhead_length) = 2240/(2240 + 26) = 0.9885

### 2. Transform Symbols into baseband I and Q samples

A QPSK modulator uses each pair of bits (non-overlapping) to decide the amplitude of I and Q components produced on it's output.
These waveforms are then passes through an FIR RRC filter.

### 3. Write samples to bytepipe using the CLI (command line interface)

Now it's time to take the baseband signal produced in step 2 and modulate it to a frequency suitable for a given channel. 
We'll do this by streaming I and Q samples to the bytepipe through it's CLI. The attached model will handle this. (!!attach model!!)

### 4. Bytepipe streams samples to ADRV9002 RF IC for modulation


### 5. Bandpass signal interacts with communication channel


### 6. Bytepipe streams demodulated samples from ADRV9002 RF IC 


### 7. Read samples from bytepipe using the CLI (command line interface)


### 8. transform I and Q samples into output bits


### 9. Compare received and transmitted symbols-->
