\# Interp4Commands - Parallel Plugin-based Commands Interpreter



Implementation of C++ script interpreter and execution engine designed to control and animate multiple objects in a 3D space. The program reads a custom scripting language (with full C-preprocessor support), parses execution blocks, and communicates with an external graphical server via TCP/UDP sockets to visualize the actions in real-time. All commands (Move, Rotate, Set, Pause, SetColor) are implemented as dynamically loaded shared libraries (plugins). The engine supports both sequential and parallel execution blocks, defined directly within the custom script. The initial state of the 3D scene and its objects is loaded from an XML configuration file, parsed using the Xerces-C++ library, and strictly validated against an XSD schema that defines object types and server communication parameters. During execution, the plugins utilize these object attributes to calculate spatial transformations and send rendering commands to the server. The syntax of these commands directly reflects the object attributes, whch are defined by an XSD schema.



\# Key Features



\- Plugin-Based Architecture: Commands are not hardcoded. They are compiled as separate shared libraries (.so/.dll) and dynamically loaded at runtime using the Abstract Factory pattern. This strictly enforces the Open/Closed Principle.



\- Concurrency Engine: Supports multithreaded execution. Parses 'Begin\_Parallel\_Actions' / 'End\_Parallel\_Actions' blocks and dispatches tasks to execute simultaneously across multiple threads, ensuring synchronized movement of multiple objects.



\- Network Communication: Implements a socket-based client to stream visualization commands to an external graphic server.



\- C-Preprocessor Integration: The scripting language natively supports C-style macros (#define) and comments, resolved dynamically before the parsing stage.



\- XML/XSD Configuration: Initial state of the 3D scene and its objects atrributes are loaded via XML and strictly validated against an XSD schema. Utilizes the Xerces-C++ library for XML file parsing.



\- Comprehensive Error Handling \& Safety: Rigorous check of XML validation. Parsing of the custom command language with clear error reporting for syntax or preprocessing mistakes. Strictly plugin verification for missing or incompatible interfaces.



\# Tools \& Platform



Language: C++



Libraries: Xerces-C++ (XML/XSD), DLopen (shared libraries), STL



Architecture: Plugin System (Dynamic Loading), Client-Server (Sockets), Multithreading



Configuration: XML parsing \& XSD validation



Build System: CMake; GNU installation package made using Autotools



\# Scripting Language Example



The engine reads custom `.cmd` scripts that define both sequential and parallel actions. Example:



```cpp

\#define ROTATE\_SPEED 30



/\* Setup initial positions \*/

Begin\_Parallel\_Actions

&nbsp;   Set Ob\_A 2 0 0 30 0 0 

&nbsp;   Set Ob\_B 10 10 5 0 0 0 

End\_Parallel\_Actions



/\* Execute complex movements simultaneously \*/

Begin\_Parallel\_Actions

&nbsp;   Move Ob\_A 10 10

&nbsp;   Rotate Ob\_B OZ ROTATE\_SPEED 60 

&nbsp;   Move Ob\_B 10 20 

End\_Parallel\_Actions

