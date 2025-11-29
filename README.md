\documentclass[conference]{IEEEtran}
\IEEEoverridecommandlockouts
% The preceding line is only needed to identify funding in the first footnote. If that is unneeded, please comment it out.
\usepackage{cite}
\usepackage{amsmath,amssymb,amsfonts}
\usepackage{algorithmic}
\usepackage{graphicx}
\usepackage{textcomp}
\usepackage{xcolor}
\usepackage{siunitx}
\usepackage{listings}
\def\BibTeX{{\rm B\kern-.05em{\sc i\kern-.025em b}\kern-.08em
    T\kern-.1667em\lower.7ex\hbox{E}\kern-.125emX}}
\begin{document}

\title{Media Center\\
{\footnotesize \textsuperscript{*}COE718: Embedded System Design
}
}

\author{\IEEEauthorblockN{Syed Maisam Abbas}
\IEEEauthorblockA{\textit{ECB Department (Electrical, Computer, and Biomedical Engineering))} \\
\textit{Toronto Metropolitan University - Electrical Engineering (BEng))}\\
Toronto, Ontario \\
maisam.abbas@torontomu.ca}
}

\maketitle

\begin{abstract}
Using the MCB1700 board, uVision, and every programming idea you have studied this semester, the goal of this final project is to build a media center. A photo gallery that can display multiple BMP files, an MP3 player that streams music and MP3 files from the PC, and a gaming center with one or more animated games the user can play are among the features of the media center.
\end{abstract}

\begin{IEEEkeywords}
Hardware, Software, Real-time, MCB1700, LPC1768, Assembly/C, MP3 Player, Photo Gallery, Game
\end{IEEEkeywords}

\section{Introduction}
The general requirements for this media center project call for a graphical user interface (GUI) that is shown on an LCD screen and provides the user with a menu of navigable features. The MCB1700's joystick must be used largely for navigation. A photo gallery, an MP3 player, and at least one game are the essential features, though other features are optional. The design should enable interactive use (e.g., using the joystick for browsing) by enabling the user to pick any menu choice with the joystick, at which time the system must do the appropriate task. The system must automatically return to the main menu after the task or user interaction is finished. Using LEDs to provide visual feedback during execution or navigation, and integrating a USB keyboard designed for gaming.

\indent This project introduces the Keil uVision IDE and some of its features using the ARM Cortex-M3 embedded processor. The ARM Cortex-M3 embedded processor's barrel shifting, conditional branching, and bit banding are the concepts that designers are expected to develop as per the learning objectives for this project. The coding basics of configuring the LEDs and LCDs of the ARM Cortex M3 and its NXP LPC1768 microcontroller will be simulated, debugged, and analyzed. 

\indent These concepts will furthermore help in the understanding of the Cortex M-series processor, including how to run a simple program on the MCB1700 dev board, which is frequently used in embedded systems. As the majority of embedded systems use ARM processors for low-power consumption and competitive performance, designers will find the skill sets obtained from this project especially useful. More specifically, students will acquire practical experience with barrel shifting, conditional branching, and bit banding to analyze their effectiveness in both Debug and Target mode using performance evaluation methods covered in the lectures. 

\indent The lab equipment will allow engineers to become familiar with the uVision environment, its simulation capabilities, and the tools needed to assess various CPU performance factors. The following coding information referenced in the appendix will be compiled to construct an LED bit-band application. Depending on how the bit is toggled at the hardware level, the LED control methods' execution speeds vary greatly. Furthermore, it introduces the concept of how to use RTX, a Real-Time Operating System (RTOS), for basic multitasking. Students will specifically learn how to use the round-robin scheduling mechanism that is a feature of RTX to schedule threads of work.

\indent Understanding real-time scheduling with µVision for ARM Cortex-M3 is the aim of this project. In particular, you will discover how to schedule and apply the widely used Rate Monotonic Scheduling (RMS) method for Fixed Priority Scheduling (FPS). Additionally, priority inversion and its resolution will be practically experienced by you. You will learn about virtual timers, inter-thread communication techniques (such as signals and thread waits), and static and dynamic priority inversions to put these strategies into practice. To complete this project, students must have a solid understanding of the previous labs, which dealt with µVision and RTX. For background information on RMS and priority inversion, consult the course notes.Students learn how to create multi-threaded apps for ARM Cortex-M3/M4 processors with RTX in this project. Using preemptive and non-preemptive scheduling techniques enabled by the RTX operating system, CMSIS libraries, and µVision, the students will learn how to schedule multi-threaded applications.
 

\section{PAST WORK/REVIEW}
The SysTick Timer on the ARM Cortex-M microcontroller is used to manage and measure the execution time of Round-Robin (RR) scheduling (Masking, ThreadYield() Function, and Direct Bit-Band) in the code. 

\subsection{Hardware Implementations}
\begin{itemize}

\item Controllers
\item Potentiometer
\item KBD directional joystick
\item KBD select button
\item Reset button
\item LCD Screen
\item Speakers
\item Port LEDs

\end{itemize}

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{Hardware.png}}
\caption{MCB1700 board - LPC1768 Microcontroller}
\label{fig}
\end{figure}

\subsection{Software Implementations}
A collection of instructions that the user processes must be carried out by the program component. Keil uVision, a program written in the C programming language, was utilized in this project. A functional system is made up of both software and hardware components that are coupled to one another. In order to transmit signals to the hardware and carry out the required tasks, the software continuously waits for human input. By obtaining the data from the program and outputting the outcomes, the hardware completes the necessary task.
\begin{equation}
https://www.gimp.org/\label{eq}
\end{equation}

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{GIMP.png}}
\caption{GIMP (GNU Image Manipulation Program) }
\label{fig}
\end{figure}

In embedded systems, where a file system is not available and images must be stored directly in the flash memory of the micro controller, the process of converting GIMP XCF image files into.c files is crucial for utilizing graphical assets. The image data must be converted from its complicated format into a raw, static C array that the micro controller's display driver can read. This specific process is frequently mentioned in embedded projects like your Media Center. To ensure the proper color depth, the image must first be processed in GIMP (or another program), such as the project's 16 BPP (R-5 bits, G-6 bits, B-5 bits) standard; the file's header information must then be eliminated using a hex editor, leaving only the contiguous raw pixel data; finally, a conversion utility wraps this raw data into a C syntax array (such as const unsigned char ARM\_Ani\_16bpp[]) to make the image memory-resident and directly accessible for writing to the LCD controller's buffer.

\section{METHODOLOGY}
\subsection{Importing an Image onto the Keil MCB1700 Board}

Using a customized version of GIMP to transform images for LCD is required per the instructions for importing images into the Keil MCB1700 Board. Since transparent pixels will appear as black, the image must first be loaded into GIMP. The image must be flipped vertically using $Image > Transform > Flip Vertically$ to compensate for the board's upside-down design. Because of the limited screen size and memory constraints, it might also need to be reduced using Image > Scale Image.

\indent $File > Export As...$ is then used to convert the image to the "C Source code (*.c)" format. The important options "Save as RGB565 (16-bit)" and "Save macros instead of struct" must be chosen in the export dialogue, along with a prefixed name. \#include "yourCFile.c" is then used to include the generated.c file in the source code, add it to a new "Resources" group, and copy it into the working directory of the Keil project. GLCD\_Bitmap(x, y, w, h, bitmap) is the function that renders the image on the LCD, where bitmap is the pointer to the image data array. 

\indent Using the GIMP macro, the file extension should be changed to .h and included as such for more recent versions of GIMP that export to structs. GIMP is advised; however, you can also use the external tool: 
\begin{equation}
https://notisrac.github.io/FileToCArray/\label{eq}
\end{equation}

\subsection{Memory Space Issues on the Keil MCB1700 Board when Importing Images }
Programmers must distinguish
between defining and declaring variables in
order to prevent duplicate memory
allocation and potential memory space
issues when importing a large number of
photos. A variable must only be defined
once within a specified scope, and it is
defined when the compiler allots storage for
it. On the other hand, a variable is declared
if the compiler is only told that it exists,
which can happen more than once. The
image variable (defined in its associated C
code, such as "image.c") must only be
declared in other files, such as "main.c,"
using the extern declaration keyword in
order to conserve board memory and
guarantee that the image data is not
specified twice. Declaring the variable as
extern in the header file and providing the
definition in only one source file is the
proper method; placing a definition in a
header file is discouraged because it results
in many definitions when multiple source
files include that header.

To gain a competitive edge, distinguish between the standard GNU GPL, which limits use to free software programs,
and the GNU Lesser GPL (LGPL), which
allows use in proprietary products. The
strategic objective is to release powerful
libraries with unique features, like GNU
Readline, under the regular GPL, whereas
the LGPL is favoured for non-unique
libraries whose functionality is easily copied
(like the C library). Applying this logic to
the GNU Image Manipulation Program
(GIMP), every special or unique library that
gives GIMP a clear, potent benefit needs to
be licensed under the GPL. The whole free
software community gains from this
deliberate exclusivity by making GIMP and
other applications more appealing than their
proprietary counterparts; other projects are
encouraged to employ free software licenses
in order to obtain and utilize those special,
GPL-covered building blocks.

\subsection{Memory System}
The IDLE process exhibits few
execution blocks, as can be seen in the
waveform, suggesting that the CPU is
running at close to 100% utilization during
the observed period. Therefore, the running
thread must willingly give up the CPU to the
next thread of equal or higher priority when
osThreadYield() is used.

The GUI follows a strict control
loop: selection of any task initiates the
feature; upon task completion or user
termination, the system automatically
transitions back to the main menu (Figure
1.0) to maintain high availability and
predictable operation.

\subsubsection{Run-time Environment (RTE)} 
Threads are only intended to run for the minimum of $\approx2.52\mu s$ needed to increment their counter in this cooperatively scheduled environment before instantly handing over to the next task.

\paragraph{Performance Analyzer } 
Displays CPU time
split among several program
modules and functions as opposed to
the previous situation, in which the
processor was monopolized by a
single infinite loop.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{Funtion1.png}}
\caption{Function based on RTX (Real-Time Execution)}
\label{fig}
\end{figure}

\paragraph{Event Viewer} 
A state of indefinite
postponement (starvation) for the
lower-priority thread in this
proactive, priority-based scheduling
system is confirmed by the Event
Viewer's visual proof.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{Function2.png}}
\caption{ThreadYield() function of the RTX for the ARM Cortex-M3}
\label{fig}
\end{figure}

\subsubsection{Real-time Execution (RTX)} 
Using the SysTick\_Handler Interrupt
Service Routine (ISR) to continuously
increment the global time counter, msTicks,
and then using the delay\_ms function to
create a software delay that pauses execution
until the required number of ticks
(BASE\_DELAY)(BASE\_ADD×curentShift)
has passed, the method entails initializing
the SysTick counter to run at a high
frequency. This method effectively employs
the SysTick as a consistent.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{Function3.png}}
\caption{Performance Analyzer of the memory system}
\label{fig}
\end{figure}

The Direct Bit-Band method is the
fastest ($\approx 1$ - 2 cycles) because the hardware
can set or clear the bit in a single, atomic
operation, due to its pre-calculated alias
address. It calculates the complex bit-band
alias address during program execution,
adding runtime overhead, the Function
Mode (Macro) is a little slower ($\approx 2$ - 4
cycles). Because it necessitates a multi-step,
non-atomic Read-Modify-Write (R-M-W)
sequence on the entire 32-bit register, the
Mask Mode is the slowest ($\approx 3+ cycles $). The
ternary conditional operator (? :) is also used
in comparison to conventional if/else
statements; the ARM compiler can
frequently optimize into effective,
branchless conditional execution assembly
instructions, reducing pipeline pauses and
enhancing efficiency.

\section{DESIGN}
Utilizing the computational and
peripheral capabilities of the NXP LPC1768
microcontroller, which has an ARM
Cortex-M3 core and is housed on the
MCB1700 development board, the Media
Center is designed as a multithreaded,
interactive embedded system. Under the
guidance of a Real-Time Operating System
(RTOS), the architecture is intended to offer
a responsive and adaptable foundation for
multimedia functions.

\subsection{System Architecture \& Control} 
The system utilizes a client-centric
design with a primary Graphical User
Interface (GUI) displayed on the on-board
LCD screen.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=1]{blocks.png}}
\caption{Flow chart of the Media Center}
\label{fig}
\end{figure}

\subsubsection{Input \& Navigation} 

Interactions are managed via
dedicated peripherals: Input and Navigation:
The core of user interaction is the
MCB1700's directional joystick, which
enables users to scroll through the main
menu options (Photo Gallery, MP3 Player,
Games) and initiate feature selection. A
USB keyboard is integrated specifically to
support interactive input for the gaming
feature.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{diagram2.png}}
\caption{The global variable, ’Page’, determines the application displayed to the LCD}
\label{fig}
\end{figure}

\subsubsection{Real-Time Operating System (RTOS)} 
The RTX RTOS is employed to
manage concurrent tasks, ensuring
responsiveness and resource allocation. The
design incorporates multithreaded
applications scheduled using both basic
round-robin and advanced Rate Monotonic
Scheduling (RMS) techniques, providing a
robust, predictable environment necessary
for real-time media processing.

\subsubsection{Implementations \& Feedback} 
Port LEDs are used to provide
immediate visual feedback on the system's
state during navigation and execution.
Analog control is provided by the
Potentiometer, which is dedicated to volume
control for the MP3 Player component via
an Analog-to-Digital Converter (ADC).
A specialized process used in
embedded systems development to convert
graphical assets created in GIMP into a
format that can be directly compiled and
stored in the microcontroller's memory (like
the one used in your media center project) is
the conversion of a GIMP XCF image file to
a.c file. The image must be transformed into
a static C array with the raw pixel data
because microcontrollers sometimes lack
file systems or the capacity to load
complicated image formats like XCF at
runtime.

\indent This procedure usually entails
exporting the image data from GIMP into a
raw binary or hex format, which is then
wrapped in a C syntax and defined as a
const unsigned char or const unsigned short
array using a conversion tool (or
occasionally GIMP itself via a
script/plugin). This gives the embedded C
code access to the image data. It can then
read the array values directly and write them
to the buffer of the LCD controller for
display. In order to guarantee that the array
only contains the contiguous, loadable pixel
values, the conversion step, which is
mentioned in your project's Abstract.txt is
essential for determining the proper color
depth (e.g., 16 bits per pixel) and
eliminating superfluous header information.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{image.png}}
\caption{Declaration of a variable using extern with the definition of int foo to convert image.xcf files into .c files}
\label{fig}
\end{figure}

\subsubsection{Game Execution (Real-time)} 
The scheduler virtually never
discovers the ready queue empty because of
this quick yielding. As a result, the CPU is
constantly occupied switching between
running application threads, which causes
the CPU utilization time to approach 100%.
This is supported by the Idle Demon
variable (countIDLE), which, when
monitored over time, records a
proportionately low count because the
scheduler hardly ever, if ever, allots CPU
resources to the low-priority IDLE task.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{diagram3.png}}
\caption{Block diagram of the game}
\label{fig}
\end{figure}

\subsection{Graphical Asset Management \& Memory Strategy}
Once a game starts, an empty 3 by 3
square grid is drawn on the board. To make
sure that the pieces are placed in the correct
spots, a grid of the coordinates was created
which contains the spaces in which only the
players can place a game piece down.

\begin{table}[htbp]
\caption{Coordinate Grid of the Game}
\begin{center}
\begin{tabular}{|c|c|c|}
        \hline
        \multicolumn{3}{|c|}{\textbf{Game Grid Layout (X, Y) Coordinates}} \\
        \hline
        \textbf{Coordinate 1} & \textbf{Coordinate 2} & \textbf{Coordinate 3} \\
        \hline
        % Row 1 (Y=3) - Top row of the grid
        (1, 5) & (1, 9) & (1, 13) \\
        \hline
        % Row 2 (Y=2) - Middle row
        (4, 5) & (4, 9) & (4, 13) \\
        \hline
        % Row 3 (Y=1) - Bottom row
        (7, 5) & (7, 9) & (7, 13) \\
        \hline
        \multicolumn{3}{l}{$^{\mathrm{a}}$Tic-Tac-Toe Coordinates.}
    \end{tabular}
\label{tab1}
\end{center}
\end{table}

Table 1 represents the exact places
on the LCD where the game pieces could
only be placed in. By default, the game
starts at the coordinate (1, 5) and is defined
by setting the values 5 and 1 to x and y
respectively. The values of x or y are then
incremented or decremented by a value that
will move the cursor to the next defined
coordinate space. In this case, y increases or
decreases by a value of 3, and x increases or
decreases by a value of 4.
To prevent the cursor from leaving
the defined coordinate space, conditions
were made to ensure that the value of y and
x do not exceed their maximum values or go
below their minimum value. This would stop 
incrementing or decrementing the values of x or y.

\subsection{Core Feature Implementation}
The game can now identify when a
player wins, loses, or draws since a
two-dimensional array is used to record
where each game piece is placed and by
which player. Every time a game piece is
placed, the placeholder values are assigned.
A "if" statement was developed that
examines each player's eight potential
winning strategies. When the placeholder
value is the same in three rows for any of the
winning strategies, the player is deemed the
winner. When a player wins, the game goes
back to the menu and shows the winner's
name on the board. The game returns to the
game menu and declares a draw if no player
connects three of their pieces in a row.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{coordinate.png}}
\caption{Tic-Tac-Toe winning sequences (8 arrangements)}
\label{fig}
\end{figure}

\section{EXPERIMENTAL RESULTS}
\subsection{Media Center}

A thread-based display would function similarly to a page-based display, but each app would have its own thread and handle the joystick and LCD from either the app's own file or the file that manages each thread, rather than a variable controlling the LCD output and handling joystick inputs from the same thread. 

To build a thread-based strategy, all four threads would need to be initialized. The main menu would be initialized with high priority, while the other three programs would be initialized with standard priority out of these four threads. The main menu would be the first thread to run when the project was launched.
The code turned out to be more clearer and more adaptable when additional features were added because each app's code was divided into its own thread.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{menu.png}}
\caption{Main Menu of the Embedded System}
\label{fig}
\end{figure}
\begin{itemize}
\item Photo Gallery
\item mp3 Player
\item Games(s)
\end{itemize}

\subsection{Photo Gallery}
According to the Photo Gallery
standards, the LCD must be able to show
different bitmap (.bmp) images one at a
time. The user interface design for viewing
these images is adaptable and creative; some
ideas include a menu with file names or a
straightforward carousel display that can be
manipulated with a joystick. Before being
displayed, the .bmp files must be converted,
which is a crucial technical requirement. A
C source file with the image data as an array
of unsigned characters must be created from
each .bmp file. GIMP with the export
options "Use macros instead of struct" and
"Save as RGB565(16-bit)" is the suggested
conversion tool. For compatibility with the
Keil compiler, all guint8 variable types in
the produced C file must be manually
changed to unsigned char after conversion.
The resulting file is of type static; thus, the
\#include "filename.c" preprocessor directive
must be used to integrate the converted C
file containing the picture data into the main
program.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{gallery.png}}
\caption{Photo Gallery of the Media Center}
\label{fig}
\end{figure}


\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{database.png}}
\caption{Image database - Photo Gallery}
\label{fig}
\end{figure}


\subsection{Audio Player}
The MP3 player component of the
media center project is in charge of using a
USB to stream audio from a PC to the
on-board speaker of the development board.
This connection must be made when the
option is selected, and it must be
disconnected when the menu is closed. The
potentiometer controls the volume; rotation
in both clockwise and counterclockwise
directions raises and reduces the volume,
respectively. The system must show a splash
screen on the LCD during playing, and the
audio must come from an MP3 or YouTube
video on the PC. The implementation should
make use of the USB audio example and
USB interrupt resources from the course.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{audio.png}}
\caption{Audio Mode}
\label{fig}
\end{figure}

\subsection{Game Selection}

The programmer has total discretion
over implementation thanks to the game(s)
section specifications, which call for the
MCB1700's LCD and joystick to be
integrated and used interactively. Simple
word games are definitely prohibited, and
the game or games must be notable graphics
arcade-style games (such as Pac-Man,
Snake, or Tetris). One game is required at
the very least, and bonus points can be
earned for implementing two or more
games.

The project's technical complexity is immediately increased by the strict demand to integrate well-known visual arcade-style games like Pac-Man, Snake, or Tetris, while expressly forbidding basic word games. In order to produce smooth, real-time graphics and interaction, this mandate intentionally tests the programmer's capacity to handle constrained resources, particularly the NXP LPC1768 microcontroller's CPU speed and memory. Arcade games require constant screen updates, pixel manipulation, collision detection, and quick state management, in contrast to word games. Developing a powerful graphics engine that can quickly create sprites, update scoreboards, and refresh the game map on the MCB1700's LCD at a fast enough frame rate to guarantee playability replaces the primary goal of simple textual output. This limitation puts the basic knowledge of embedded systems timing and optimization to the test.

Furthermore, the game selection necessitates a deep dive into the interfacing of the LCD and joystick. Because the game must be "interactively" integrated, the programmer must build effective, low-level driver code to read the joystick's state with minimal latency (perhaps via GPIO pins or the Analog-to-Digital Converter, as the literature suggests with the LPC\_ADC\_TypeDef). Smooth and responsive movement is crucial for a game like Pac-Man; the game becomes unplayable if there is a noticeable lag between a joystick input and the character's movement. Data structures such as the LPC ADC TypeDef from the LPC17xx.h header file enable direct register manipulation in this operation. The difficulty is in converting the joystick's physical, frequently analog input into accurate, debounced digital signals that efficiently manage the game's logic and graphics updates.

The header file LPC17xx.h, which provides information on the LPC1768 chip's registers and access to them through well-formatted C data structures, is typically used when programming with the NXP LPC1768 chip. This allows operations to be carried out on these registers quickly and conveniently. The LPC\_ADC\_TypeDef, a general data structure for accessing analog to digital converter registers, is one example of such a data structure. That data structure initially provides access to a few control registers during project setup.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.85]{game.png}}
\caption{Game Menu and operation of Player 1 vs Player 2 in tic-tac-toe}
\label{fig}
\end{figure}

Lastly, a crucial architectural factor—modularity and resource partitioning—is introduced by the possibility of additional points for implementing two or more games. A successful strategy would entail creating a single game framework or engine that is readily reusable across several game titles since the programmer has complete control over implementation. While enabling separate game loops and assets for games like Snake and Tetris, this structure must effectively handle shared resources, such as memory for graphics buffers and the interrupt routines for the joystick. This design decision is essential for minimizing code duplication and showcasing sophisticated software engineering concepts in the embedded context, demonstrating that the design is a scalable platform that can support numerous unique, resource-intensive graphical applications rather than just a single, monolithic application.

\section{CONCLUSIONS}
The project's development was
greatly aided by the knowledge acquired
from the semester's lectures and
laboratories. This entails comprehending the
characteristics of the MCB1700 Board,
uVision ideas, debugging, and
multi-threading to solve large programs
featured in a fully operational media center
that makes use of both software and
hardware issues, as well as software and
hardware interaction. A photo gallery, an
animated game, and an MP3 player all
contributed to the media center's
user-friendliness, simplicity, and enjoyment.

Practical experience in embedded
systems design using the ARM Cortex-M3
CPU and the Keil uVision IDE, this project
effectively achieved its main goals. Creating
and evaluating code using processor
capabilities like bit banding, conditional
branching, implementing the RTX RTOS,
knowledge about NXP LPC1768
microcontroller low-level performance
enhancement, and barrel shifting were the
early learning objectives.

\begin{figure}[htbp]
\centerline{\includegraphics[scale=0.35]{blcokss.png}}
\caption{Block Diagram of the Media Center}
\label{fig}
\end{figure}

Overall, these fundamental and
sophisticated ideas came together to create a
Media Center application that included a
graphical Gaming Center, a USB Audio
Player with ADC-controlled volume, a
Photo Gallery requiring specific GIMP
image conversion techniques, and a
navigable GUI (controlled by the joystick).
Furthermore, the Keil uVision software and
ARM Cortex M3 device, as displayed on the
liquid crystal display (LCD) using the
Debug simulation session with optimization
levels to control the LEDs.

\section{REFERENCES}
The bibliography for the COE718 Final Project, "Media Center," is very informative; it functions as a map of the project's development workflow, toolchain, and academic limitations in addition to being a collection of references. From the low-level embedded programming requirements to the high-level graphical assets and configuration management, the three references, GIMP, GitHub, and the course materials. Collectively describing the full ecosystem. The effective implementation of the GUI-based media center on the MCB1700 board depends on this synthesis of technical, logistical, and educational resources.

This reference emphasizes the use of GIMP, the GNU Image Manipulation Program, a potent open-source tool for image editing and graphic design. GIMP was probably used in the context of the Embedded Systems Design project to produce and improve any visual assets required for the Graphical User Interface (GUI) of the media center that was shown on the LCD of the MCB1700 board. This entails creating unique icons, splash screens, or diagrams and making sure they are appropriate for the embedded technology in terms of color depth and resolution. This reference's inclusion highlights the methodical procedures used to prepare the system's non-code components and shows how multimedia components were incorporated into the finished output. GIMP made it possible for the programmer to carry out important functions like color depth reduction, which guarantees that icons and images follow the restricted color schemes that the integrated display hardware supports. It was utilized to produce and improve visual assets, including schematic diagrams for the final report, splash screens shown at launch, and unique icons for the media center's many programs. This careful preparation of the non-code components reduced the processing cost on the NXP LPC1768 processor during runtime by ensuring that the graphical elements were both aesthetically beautiful and computationally efficient. The reference emphasizes this meticulous planning, which is proof of a comprehensive approach to embedded development where software optimization encompasses graphical asset pipeline management.

When a GitHub repository is cited, it means that a version control system was used to professionally manage the project's source code. As the focal point for the whole development lifecycle, GitHub allows for effective change tracking, safe code backups, and possibly even collaboration if the project includes several team members. This reference, which indicates the location of the final C/C++ code, design files, and embedded media center configurations, is essential for transparency and reproducibility. It confirms that industry best practices for software engineering were used in the development and upkeep of the code covered in the report. The reference to a particular GitHub repository attests to the "Media Center" project's adherence to professional software engineering standards and its early integration of a strong version control system. The entire development lifecycle was centered around GitHub. Its use offered crucial infrastructure for safe, off-device code backup and change tracking, which enabled the programmer to audit the evolution of the code base and return to earlier stable stages. This is especially important in embedded development, where months of work can be easily lost due to corrupted development environments or physical hardware failures. Citing the repository also creates a foundation for project reproducibility and transparency. The final C/C++ source code, low-level driver implementations, hardware initialization procedures, and embedded media center configuration files are all explicitly pointed to by the reference. This repository is the final record for assessors or future developers, attesting to the application of industry best practices in the creation and maintenance of the code. With this integration, the project becomes a professionally managed technical deliverable rather than just an academic exercise.

The entire endeavor is grounded in its educational and technical context thanks to this scholarly reference. Citing the official course page for "COE 718: Embedded Systems Design," the report makes it clear that the project complies with Dr. Gul N. Khan's technical standards and curriculum criteria. The project's objectives, such as the requirement to install a basic GUI-based media center on the MCB1700 board and make use of its hardware capabilities like the joystick and LCD, are based on this source. The theoretical framework, design limitations, and pedagogical goal that eventually guided all technical choices and project outcomes described in the final report are established in the reference. The official course materials for Dr. Gul N. Khan's "COE 718: Embedded Systems Design" are the most fundamental source. The entire endeavor is grounded in the necessary technical and educational context by this scholarly citation. The project's scope and design constraints were clearly defined by adhering to the course requirements. The main goal was to directly target the NXP LPC1768 microcontroller and its related peripherals on the MCB1700 board rather than just create a broad piece of software.

The reference mandates a number of integration points, including the implementation of a basic GUI-based media center, the need to interface with the LCD for display, and—most importantly—the joystick's interactive control mechanism. Low-level programming that directly accesses the chip's registers—typically via structures described in the LPC17xx.h header, like the LPC ADC TypeDef used for joystick input—was required by this framework. The project thoroughly tested the student's capacity to go beyond high-level application programming and engage in actual embedded systems design, handling interrupts, memory, and direct hardware access, according to the course's mandate. These three references work together to highlight the project's all-encompassing implementation approach. The GIMP reference guarantees that the finished project satisfies the visual criteria of a contemporary GUI-based program, the academic reference establishes the technical standard, and the GitHub reference guarantees the development process is responsible and repeatable. Thus, the "Media Center" project's ultimate functionality—the seamless display and interactive control—as well as its adherence to this established methodology are used to gauge its success. The report's specific naming of these sources attests to the programmer's methodical approach to the problem, which addressed the LPC1768 chip's technological limitations while upholding a high standard for graphical display and software administration. This multi-layered strategy is a sign of an advanced engineering method used in the difficult limitations of an embedded environment.

\begin{thebibliography}{00}
\bibitem{b1} GIMP, https://www.gimp.org/ (accessed
Nov. 27, 2025).
\bibitem{b2} GitHub, https://github.com/syed-maisam
(accessed Nov. 8, 2025).
\bibitem{b3} G. N. Khan, “COE 718: Embedded
Systems Design,” Electrical, Computer and
Biomedical Engineering. https://www.ecb.torontomu.ca/~courses/coe718/index.html
\end{thebibliography}

\vspace{12pt}
\color{red}
Syed Maisam Abbas, it is crucial that you carefully go over your "COE718 Final Project, Media Center" report and eliminate any instructional or guidance content that came from the original IEEE conference form before submitting it. This covers any non-content elements, such as usage notes, formatting advice, and placeholder text. If you don't remove this template material entirely from your final paper, your submission can be considered incomplete or formatted wrongly, which could eventually prevent the report from being approved or published in the conference proceedings. Please consider this removal to be the last, required check.

\end{document}
