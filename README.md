# Yomi
Yomi is a C++ 3D game engine project built entirely from scratch, with the intent to learn what goes into the heart of a game engine.

## Game
A space game is included in this project which is built using the engine, currently a work in progress.

See the end of this document for a bunch of videos which show off the development progress.

## How to Build and Run
Clone with:
```bash
git clone --recurse-submodules https://github.com/Aleksbgbg/Yomi
```

For UNIX systems (MacOS, Linux), download and install the [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/), cmake, and C and C++ compilers on your system if needed, then run these commands from the project directory:
```bash
cmake -B cmake-build-debug
cmake --build cmake-build-debug -j 8

cd cmake-build-debug/src && ./yomi
```

On Windows, open the project in Visual Studio and run it.

Alternatively, on any platform use JetBrains CLion which will automatically figure out building and running for you, as long as you have the dependencies listed above installed.

## Dependencies
Yomi has very few dependencies:
- C++ standard runtime library
- Vulkan (for its rendering backend)
- GoogleTest (for unit tests where appropriate)
- SDL (for cross-platform abstraction and windowing)
- FreeType (for text rasterization)
- glm (for 3D math and feeding data to the GPU in the correct layout)
- stb_vorbis (for decoding vorbis audio files)

glm will definitely be removed in the future, and stb_vorbis could potentially be removed in the future (in favour of a custom OPUS implementation) if time allows. However, the other dependencies would take too much time to implement from scratch.

## Portability
Yomi is portable and currently runs on Windows, Linux, and MacOS. More platforms may be added in the future and the porting process is fairly straightforward.

## Points of Interest
Many interesting programming problems have been solved during the development of Yomi.

Some of my favourites are listed below, in no particular order.

### UI Framework
Yomi has a built-in UI framework which takes large inspiration from Microsoft's WPF framework, using a similar XML syntax, as well as binding syntax, and encouraging use of the MVVM (Model-View-ViewModel) pattern. Everything in the framework is built from scratch, including the XML parser. Calls from the ViewModel can update the view and changes in the view are fed back to the ViewModel.

See the example [pause menu in XML syntax](/resources/ui/pause_menu.xml), as well as the [ViewModel](/src/game/viewmodels/PauseMenuViewModel.cc).

When deciding the rendering style of the menu, I went with a near-identical copy of GTA V's pause menu. GTA V is one of my favourite video games, and the GTA franchise has been a big inspiration whilst developing my programming and mathematics skills. I've been impressed for years at the amount of detail and technological marvel in GTA V at launch, and it has been a dream of mine to one day lead the development of a programming project with reach as large as GTA, and use the reach to have a positive effect on its audience. I've never tried implementing anything from GTA, though I've always wanted to see if I'm up to the challenge - so I thought the pause menu would be a good start.

A video of the pause menu working is included at the top of the videos section.

### Scene Composition Syntax
An interesting problem I faced during development was describing the game in an abstract way which isn't coupled to the renderer implementation or the structure of the code. Pondering on how to solve the problem, I looked to Unity's actor, property, and script model. You create actors in the scene, which have different properties such as colliders (which themselves have a bunch of settings), and then you can program how these objects interact with scripts.

I created a similar system, albeit much simpler and written in C++ instead of edited in a UI. You can add actors to the scene, which have properties such as visibility and a transform (position, scale, rotation), and these properties can be modified through 'behaviours' (scripts).

Here is an example of adding an NPC spaceship which moves constantly forward:
```cpp
scene_.Actor()
  .Attach(BEHAVIOUR(ConstantMovement,
                    ConstantMovement::ParameterPack()
                        .SetTransform(actor.RetrieveProperty<Transform>())
                        .SetForwardVelocity(1.0f)))
  .Mesh(npcMesh)
  .Child(spaceshipExhaust)
  .Spawn();
```
`BEHAVIOUR` is a macro which creates a lambda function that creates instances of behaviours, so that you can spawn an actor composition multiple times. Each time you call `.Spawn()`, the actor receives new instances of all the behaviours and children set on it with previous calls. The lambda accepts two parameters that you have access to when creating behaviours, `actor` and `parent`, which are the current actor and it's parent, respectively. You can use these actor instances to retrieve properties from them that you would like to modify in a script, thus allowing arbitrary programs to be applied to objects in the game.

The ConstantMovement behaviour can have parameters set on it, such as the forward velocity of the object, which makes it reusable. It could also move any actor with a transform, whether it be the current actor, or it's parent. You can see that in the example above, the constant movement is applied to the current actor's transform.

You can see the [Scene](/src/game/Scene.cc) constructor where the entire game is described using this syntax.

### PNG Decoder
Near the start of the project, I was using bitmap images for textures but noticed that those take up a lot of space. Deciding that at some point I would need to implement PNG images to save space during deployment, I deep dived into implementing a [barebones PNG decoder](/src/core/files/png/png.cc) for the textures I was using, based directly off of [the PNG specification](https://www.w3.org/TR/2003/REC-PNG-20031110/), [the ZLIB specification](https://www.ietf.org/rfc/rfc1950.txt), and [the DEFLATE specification](https://www.ietf.org/rfc/rfc1951.txt), without any other supplementary material.

The minimal implementation took a week to write as some parts of the specifications were particularly tricky to understand (in particular the encoding format in DEFLATE). The decoder is not particularly efficient, as the decoding algorithm is used directly as described in the DEFLATE specification. Some other implementations like the LiteSpeed Web Server use [fast huffman decoding](https://blog.litespeedtech.com/2019/09/16/fast-huffman-decoder/) (via more efficient data structures) which drastically speeds up the decoding process. Decoding a PNG texture in the game takes about 1 second in debug, which is unsatisfactory. In the future, I will adjust the implementation to use better data structures to achieve fast huffman decoding.

### Key Generator
Keys were required to keep track of resources and actors in a scene, so that resources can be released by key when they run out of scope, and actors can be deleted from the scene graph when they are despawned.

I wrote a [key generator](/src/core/algorithms/KeyGenerator.cc) which encodes the current time, thread ID, and sequence number (a number which increases on every key generation) into a 64-bit number to use as a key, with the bit widths of these values set so that the generator could run in a loop on multiple threads and still not produce duplicate keys (on modern hardware). The code is well abstracted, so the bit width of the number and encoded values could be varied and optimised for a particular use if necessary. 64-bit was just an easy choice without much optimisation.

The idea is based on [Twitter's snowflake format](https://blog.twitter.com/engineering/en_us/a/2010/announcing-snowflake). Although it may be slightly overkill since there is no multi-threaded key generation in the game and separate instances of the generator are used where needed (so clashes are unlikely in the first place), it was fun to implement and play around with this idea. As the game stands, the key could have been implemented just as a monotonically increasing sequence and sufficed.

### GPU Memory Allocator
Since Vulkan is such a low-level API, it requires manual management of GPU memory. Although most projects might use [Vulkan Memory Allocator (VMA)](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) due to it's high quality of implementation, I decided I wanted to write my own memory manager for learning purposes.

For simplicity, my memory manager is a growing allocator (no deallocations, only further allocations) which starts off with a memory block of a base size, then allocates additional blocks of 1.5x the size of the previous block if required. It tries to allocate free memory first if possible, before allocating bigger blocks. The memory manager implementation lives [here](/src/memory), and is called by Vulkan code from a wrapper [here](/src/renderer/vulkan/api/memory/DeviceMemoryAllocator.cc).

### Networking
The [archived networking branch](https://github.com/Aleksbgbg/Vulkan/tree/archived/networking) demonstrates the first working trial of a multiplayer implementation of the game, where players can join the same server and fly together. Although the implementation was very naive and unoptimised (and was only implemented on Windows), it worked really well and was very fun to make and play test with friends. In particular, I was surprised to find that playing with my friends in South Korea (very far from the UK!) worked without any noticeable lag! In the future, networking will be implemented again, this time using server time steps and client-side prediction.

### Vulkan Struct Builder Generation
I wrote a light object-oriented (RAII) wrapper on top of Vulkan to somewhat isolate my C++ code from the C-style API. One of the main issues I wanted to address was filling out structs in C-style - I wanted to be able to fill in struct parameters inline, in the same statement as my Vulkan calls, and also sometimes modify these parameters as they pass through the call stack of my Vulkan wrapper. To do this, I used the builder pattern, creating a builder class for every Vulkan struct I use, which has methods setting each of the struct's values. I took extra care to ensure the structs were composable, so that nested structs could be set in one full, readable statement. This was an excellent solution, resulting in some [very readable code](https://github.com/Aleksbgbg/Yomi/blob/331273a1e47d9b509a2de6c8dac6fba355ed188f/src/renderer/vulkan/Vulkan.cc#L364-L411) when creating Vulkan objects.

However, creating over 70 individual structs manually seemed too time consuming. Instead, I wrote [a small Python script](/create_vulkan_struct_builder.py), which given a Vulkan C-style struct definition straight from the specification, generates the relevant builder class. It worked great and as a result I have very readable Vulkan structs.

All of the structs are in [this directory](/src/renderer/vulkan/api/structures). The structs are generated with macros (defined in [this file](/src/renderer/vulkan/api/structures/define_structure.h)) to allow me to modify the bodies of all the builders at the same time. A bonus advantage of these structs is that template objects can be built ahead of time, then used with a builder and have only some properties customised. For an example, see [this file](/src/renderer/vulkan/api/structures/default.h) which contains all of my frequently used template objects, and [an example](https://github.com/Aleksbgbg/Yomi/blob/06e5f83d119f1296e6f4ae6346d4386675c94a72/src/renderer/vulkan/Vulkan.cc#L764-L769) of them being consumed by a builder and then modified further.

## Unsolved Problems
Although this project aims to be a complete implementation of a game engine from scratch (within reason), two main problems remain unaddressed at the time - multithreading and CPU memory management.

### Multithreading
Although the game does run multiple threads (one for event processing and one for rendering, as well as additional threads for networking when that is implemented), there is no overarching multithreaded design where calculations for actors in the game can be run in parallel or GPU command buffers can be generated in parallel.

Although this is an interesting problem to solve, it would require deep design thought and a very time-consuming implementation with many cross-cutting concerns, whilst at the same time not being necessary due to the simplicity of the implemented game.

Still, there is space to solve this problem in the future should the game become inefficient when running single-threaded.

On a related note, heavy computations such as the particle exhaust from a spaceship are run multithreaded on the GPU via compute shaders, so no heavy processing is currently done by the CPU.

### CPU Memory Management
Although the project contains a GPU memory manager, there is no CPU memory manager which optimises memory allocations and deallocations for the engine. Everything is allocated through the C++ standard runtime library. Surprisingly, even without optimising this process, memory allocations are pretty fast and my attempt at writing a CPU memory manager was slower than the standard library, so there is no reason to spend time solving this problem at the moment.

## A Note on Consistency and Conventions
As the project was developed, I learnt more about C++, which evolved my programming conventions. As a result, the style in the project has changed several times, so some files may be inconsistent with others. I use the most appropriate conventions based on my latest knowledge when I add new code, but porting all the old code each time I learn something new would be too time consuming. I mostly refactor code only when I am editing it for other reasons in the first place, so generally files edited a long time ago will be out of sync with the latest conventions.

## Branch Naming
Branches in this project follow a defined naming scheme.

The main branch contains the latest code that builds and works as intended.

wip/ branches are used for working on multiple features simultaneously if required, and will eventually be merged into main.

archived/ branches contain functionality of historical interest. They are used to keep features that will be reworked and included in the game in the future or features that won't make it into the final version of the game.

obsolete/ branches contain abandoned code for reference. The code may be reworked and included in the game in the future, but for the time being isn't worth working on.

## Videos
Here are some videos of the space game in action. Videos at the top are the most recent. Some videos have sound, but lower your headphone volume as they can be quite loud.

https://user-images.githubusercontent.com/31630318/154769437-04c9dee9-5877-4bce-984e-b9fb7b9409be.mp4

https://user-images.githubusercontent.com/31630318/154769634-1a03091b-ba00-4a0a-b922-9dc2f641748c.mp4

https://user-images.githubusercontent.com/31630318/154769643-77819eb5-8159-455f-aa9d-e036263096a0.mp4

https://user-images.githubusercontent.com/31630318/154769717-3f25c95c-fcf0-4418-9a7a-870f54fced6b.mp4

https://user-images.githubusercontent.com/31630318/154769728-fe4fbd1f-decb-4287-b441-035b089e4b2f.mp4

https://user-images.githubusercontent.com/31630318/154769749-86e015d2-c235-4a8e-a8e0-8d4814b8f3db.mp4

https://user-images.githubusercontent.com/31630318/154769769-1618422b-efe6-431b-a1ad-21a3a3da52c8.mp4

https://user-images.githubusercontent.com/31630318/154769975-6865a45a-cbb6-4c2e-b3ba-ee81e95bed44.mp4

https://user-images.githubusercontent.com/31630318/154770000-a6f7604b-3ab6-480a-8e2a-033042f7ff8a.mp4

https://user-images.githubusercontent.com/31630318/154770130-0ceb5fe8-c883-4386-bedc-8176f10a54bf.mp4

https://user-images.githubusercontent.com/31630318/154770144-9a3c6105-c503-4587-bf5f-4cda2eaff2a1.mp4

https://user-images.githubusercontent.com/31630318/154770190-051357b0-d536-4eb9-a83b-957722f0341d.mp4

https://user-images.githubusercontent.com/31630318/154770195-a630fba4-c055-404a-bc0b-c0dc9ab3c0b9.mp4

https://user-images.githubusercontent.com/31630318/154770204-d8917720-5d4c-42c3-9cf5-bade7da7d485.mp4

https://user-images.githubusercontent.com/31630318/154770223-6fad850f-e5a5-4775-9a79-dbf9a2c97c6e.mp4

https://user-images.githubusercontent.com/31630318/154770119-78443025-23b7-45af-8a78-a1da6309f6c7.mp4

https://user-images.githubusercontent.com/31630318/154770234-124d1a88-f8c9-4e3f-937c-ec8a3af63d51.mp4

https://user-images.githubusercontent.com/31630318/154770237-cbea733c-44a4-4f80-8915-8a6bc7d4c35f.mp4

https://user-images.githubusercontent.com/31630318/154770246-b1809db4-e4fa-43e5-9bc4-c2675f96e0c6.mp4

https://user-images.githubusercontent.com/31630318/154770258-a54c1e3c-4d3f-4c98-9e36-229a3e1a5106.mp4

https://user-images.githubusercontent.com/31630318/154770270-d7da9583-6d67-4597-8c65-7236cd87ce9b.mp4

https://user-images.githubusercontent.com/31630318/154770277-25ebe755-e4b2-4a7f-8601-1f758763cdd1.mp4

https://user-images.githubusercontent.com/31630318/154770286-b523dbcd-e94e-477b-8fb1-2c867a889ea9.mp4

https://user-images.githubusercontent.com/31630318/154770321-f02efeeb-fd4d-400f-97db-09f99cda6519.mp4

https://user-images.githubusercontent.com/31630318/154770337-370c09fb-b11a-4e86-b466-ffe2f6acab54.mp4

https://user-images.githubusercontent.com/31630318/154770342-19d1c867-c0e2-4133-a307-a381e78eb17e.mp4

https://user-images.githubusercontent.com/31630318/154770242-5f6977de-c834-4d83-b2e7-d42eb1b60c7c.mp4

https://user-images.githubusercontent.com/31630318/154770364-c493db42-0f45-4f43-9f79-72f8f9b44872.mp4

https://user-images.githubusercontent.com/31630318/154770357-f0668257-c0c6-451a-a1cb-26d27a6d890f.mp4

https://user-images.githubusercontent.com/31630318/154770367-566400c5-d2d5-499a-b732-f149cc9f6278.mp4
