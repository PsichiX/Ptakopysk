Ptakopysk
=========
![Ptakopysk](https://raw.github.com/PsichiX/Ptakopysk/master/media/logo-mini.png)
###C++ game prototyping toolset based on component system###
**[DOWNLOAD LATEST RELEASE](https://github.com/PsichiX/Ptakopysk/releases)**

---------
###Zasuvka Ptakopyska IDE###
![ZasuvkaPtakopyska](https://raw.github.com/PsichiX/Ptakopysk/master/media/zasuvka-ptakopyska-screenshot.PNG)

---------
###How-to:###
All you will want to do with this library in your project is just use Zasuvka Ptakopyska IDE:
- Open IDE executable: **\<sdk\>/IDE/ZasuvkaPtakopyska.exe**
- In **Settings** page check if **Code::Blocks** and **bash.exe** paths are valid.
- **Create** new or **open** existing project.
- **Edit** code, **build** and **run**!

Or use project generator script:
- **cd \<sdk\>/templates/**
- **./make_new_project.sh -o "path/to/project/directory/" -p "ProjectName"**
- Pro-tip: You may also want to change location of include and library files in Code::Blocks project.

Or use templates directly:
- **main:** [TemplateMain.cpp](https://github.com/PsichiX/Ptakopysk/blob/master/templates/TemplateMain.cpp)
- **components:** [TemplateComponent.h](https://github.com/PsichiX/Ptakopysk/blob/master/templates/TemplateComponent.h), [TemplateComponent.cpp](https://github.com/PsichiX/Ptakopysk/blob/master/templates/TemplateComponent.cpp)

---------
###Examples:###
- [Floppy Disk](https://github.com/PsichiX/FloppyDisk)
- [Math isn't heartless](https://github.com/PsichiX/Ptakopysk/tree/master/examples/MathIsNotHeartless)
- [Progress ring shader](https://github.com/PsichiX/Ptakopysk/tree/master/examples/ProgressCircleShader)

---------
###Using 3rd-party libraries:###
- **Xenon Core 3** - base code structure
- **SFML 2.1** - graphics & audio
- **Box2D** - physics
- **JsonCpp** - serialization

---------
###About used environment:###
- **Compiled with:** GCC (tdm-1) 4.7.1
- **IDE:** Code::Blocks
