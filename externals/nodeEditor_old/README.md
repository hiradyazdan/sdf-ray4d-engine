The node editor is made by Dimitry Pinaev.
[Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor

As the project has gone along, it is hard to exactly pinpoint what was modified in the code.
The significant modifications to the node editor are explained our report in Section 5 called Implementation.
The logic behind these modifications tries to follow the original idea by the developer of this
node editor so it does not necessarily follow the NCCA Coding Standard. Another reason why it is
hard to pinpoint any specific modifications in the node editor code, is because adjustments were made
throughout the project to have the editor suit our needs. Most of these modifications were smaller ones
like exposing the nodes to the scene view for traversal etc. The node editor also consists of multiple files that
connect to make a whole, sometimes to adjust a simple thing in the functionality of the editor, it was necessary
to modify one to few lines in almost every single file. Thus we've opted to not explicitly point out what
was modified in the code. But to keep a clear distinction between what's fully done by us and what's made by Dimitry and
modified by us of the node editor; all of the node editor functionality files are found inside the directory
called nodeEditor, which is located at the root of the program's directory and where this README is loated.
Everything inside src/ and include/ is written by us, the files inside src/SDFGraph/ and include/SDFGraph/ are based on
the example nodes by Dimitry Pinaev, but are each written to serve a specific purpose for this project.
All of the original files by Dimitry Pinaev have the following comment appended to the relevant header-file:

/// @brief Node Editor
/// Dimitry Pinaev.
/// [Accessed November 2016]. Available from: https://github.com/paceholder/nodeeditor

If modifications were made to that header-file and/or its source, the following is appended to the above comment:

/// Modified by Teemu Lindborg & Phil Gifford

Some modifications made to the editor:
- Initially the editor lacked the ability to make multiple connections from a single output,
  which we implemented as it was one of the requirements to really have the software be
  parameterisable. After sending our implementation to Dimitry Pinaev, he informed us that he
  usedsome of our ideas in his implementation of the multiple connections per output.
- Again initially the node editor lacked the possibility to change the colours of the node ports
  or connections. We implemented this to give a clear visual cue to the user of what can be connected
  where. This has later been implemented in the node editor, but the one we use is still our modification.
- As the collapsed node is not an "actual node", but rather a way to reference other nodes some parts of
  the save and load functionality, viewing of the node scene etc had to be modified. This is explained more
  in the report.
- We wanted to nodes to be categorised for easier access when creating them, so we had to rewrite/modify
  some of the way the nodes are registered and displayed in the menus.
