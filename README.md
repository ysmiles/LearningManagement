# LearningManagement

This repository is oringinally used for Yu Shu's record of 2017-Spring CMPE-207 project.

It is a network based learning management system. (something like Canvas or Blackboard).

The overview

![img](/image20.png)

At this stage, we can divide our whole system as 3 key part.

The general message transfer server. This is the core.

The MySQL database system to save information. This database is running remotely at a VPS (virtual private server) provided by DigitalOcean.

The RPC file handling subsystem for file transfer and store. (currently, it runs locally as the core server. But in the future, it could be implemented at other machine to achieve higher level of distribution.

When the Server starts to run, it first fork once to create a RPC file sender (version 1) to handle files sending. Client can call RPC receiver to download files.

At the client side, when we open a client, it also fork once to run a RPC sender (version 2). When an instructor want to upload files, actually the client would send a command to server and let server download files from client side.

And for each instructor the file should be automatically uploaded to the specific folder.
