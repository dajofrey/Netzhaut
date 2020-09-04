\page Target Target

<div style="width:700px;">

This target document is supposed to be development guideline as well as (planned-)feature
overview. It's updated/revised quite frequently.
Because the project is open to ideas and changes in general, some targets may change
drastically over time, though the overall principles will remain intact.
These principles are to always strive for **quality**, **user friendliness** and **innovation**.

\section Interface Interface

**Hackable and Extensible**  
Netzhaut tries to have as little as possible happen under the hood. What this means in practice, is that Netzhaut shifts functionality from C code to JS code wherever it makes sense.
The solution is slower than the compiled alternative, but ultimately much easier to work with from a dev standpoint.
For example, this way users can provide custom scripts to change functionalities which otherwise would not be changeable.  
<br>
**Extended JS**    
Netzhaut extends the possibilities of JS by allowing the developer to query additional client-side data. It also adds other API's to configure all sorts of stuff related to Netzhaut.  
<br>
**Security**  
The extension of JS is of course a no-go if you want to fetch a web-page from an unknown source. For this use-case, Netzhaut can be configured to run in a secure mode.
This strips away any potentially harmful JS features. In general, security is under review at all times.  

\section Internal Internal

**Performance**  
While performance may not be a priority, it for sure won't be neglected. If something needs to be addressed performance wise, it will be.
Generally, if code could run faster another way and it won't deteriorate code readability, it needs to be changed.
If however it would make the code less readable, the change is up for debate.  
<br>
**Multithreading**  
Multithreading is intrinsic to Netzhaut's design. The user can scale physical thread usage at any time. Window threads instruct the underlying render API to render all corresponding tabs.
Tab threads create/handle all resources necessary for rendering.  
<br>
**Windows and Tabs**  
Netzhaut creates windows. These are you typical OS-provided window containers where you can render stuff into. These windows can contain any number of tabs. Tabs can be understood as your typical browser tab.
So similiar to a browser tab, Netzhaut will render stuff into it, e.g. a HTML document. These tabs can be arranged at free will using a JS API.   

\section Rendering Rendering

**Flexible**  
The engine is flexible in the way it handles its backend rendering dependencies and has a adaptive nature. To allow this, Netzhaut is able to use more than one possible backend for rendering.  
<br>
**VR Support**  
Netzhaut shall support VR in some way. This hasn't been thought out yet. 

\section Building Building

**Build-System**    
Netzhaut uses its own build-system, focusing on a lightweight but powerfull solution.   
<br>
**Lightweight**  
The project tries to avoid big dependencies and generally only downloads/installs external sources when the target doesn't provide them by itself.  
<br>
**Cross-Platform**  
Building is supported on Linux and Windows. Support for MacOS is written in the stars somewhere and I'm not sure if it's gonna happen.

\section Debugging Debugging

**Essential**  
Debugging is an essential feature and is supported by most sub-systems. For performance reasons, developers need to explicitly specify a flag at build-time to use more advanced debugging routines.   
<br>
**TTY**  
Netzhaut has a cross-platform TTY interface used for monitoring, controlling etc..   

</div>
