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
  1. Run the following command in the matlab command window:
```
openExample('comm_hdlcoder/QPSKTxRxHDLExample')
```
  2. Double-click on "commhdlQPSKTxRx.slx" to open the simulink model associated with this example
  3. Enter the following text in the matlab command window and hit "Enter" to run the example simulation script:
```
QPSKTxRxHDLExample
```
### 2. Generate HDL
We'll be generating two sets of HDL code. One for the QPSK Tx block and one for the QPSK Rx block. Let's start with Tx.
  1. Double click on the QPSK Tx block to see the system inside
![image](https://user-images.githubusercontent.com/122583249/215589241-69d8ac15-4ebf-479c-8d40-476930ab5a0f.png)
  2. Click somewhere in the blank background to make sure no individual block in this scope is selected
  3. Click the thumbtack button next to the "Code for" field to make sure you're generating code for the QPSK Tx block. 
  It should look like this when you're done:<br>
 ![image](https://user-images.githubusercontent.com/122583249/215590267-54345150-1b8c-4d64-9408-0e6275ce6ba1.png)

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
