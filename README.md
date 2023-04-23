# RePulsar
## Описание
Это PoC реализция SMB-бэкдора, основанная на собственном исследовании ядра Windows и механизма SMB. Так как это всего лишь PoC, особого функционала нету. В будущем планируется поддержка базового функционала:
| Функция  | Статус |
| -------- | ------- |
| Heresy's Gate инъекция кода  | :heavy_check_mark:   |
| Чтение SMB тега | :heavy_check_mark:     |
| Прямое чтение/запись вирутальной памяти    | :heavy_check_mark:    |
| Выполнение произвольного Usermode-шеллкода   | :heavy_check_mark:    |
| Передача Usermode-шеллкода в SMB пакете   | :heavy_check_mark:    |
:warning: - работа ведется, :heavy_check_mark: - функция завершена, :x: - работа пока не начата/приостановлена


Также планирутеся подробное описание работы данного механизма и оптимизация кода.
На текущий момент стабильно работает на:
- Windows 10 1809
- Windows 10 2004
- Windows 10 21H2

Вдохновлено проектом [smbdoor](https://github.com/loneicewolf/smbdoor)

## Функционал
На текущий момент бэкдор обладает возможно выполнять произвольный Usermode-шеллкод путем инжекта и создания потока в целевом процессе (по умолчанию explorer.exe). Были протестированы кастомные calc-шеллкоды и шеллкоды от Metasploit (стейджер), ограничения на размер шеллкода - 4000 байт (при большем размере может работать нестабильно, но ограничение можно изменить в коде). Шеллкод достаточно скопировать в файл shellcode.h в проекте ping (он должен быть в формате строки). Если хотите чтобы у шеллкода были SYSTEM-привилегии, то инжект делайте в winlogon.exe. Все функции протестированы в x64, в x86 системах что то может вовсе не работать. 

## Description
This is a PoC implementation of the SMB backdoor based on the implementation of the Windows kernel and the SMB mechanism. Since this is just a PoC, there is no special functionality. In the future, it is planned to support the basic functionality:
| Function | Status |
| -------- | ------- |
| Heresy's Gate code injection | :heavy_check_mark: |
| Read SMB tag | :heavy_check_mark: |
| Direct read/write of virtual memory | :heavy_check_mark: |
| Executing a usermode shellcode | :heavy_check_mark: |
| Sending usermode shellcode in SMB packet | :heavy_check_mark: |
:warning: - work in progress, :heavy_check_mark: - work completed, :x: - work not yet started/suspended


A detailed description of the operation of this mechanism and code optimization is also planned.
At the time of its appearance, it works stably on:
- Windows 10 1809
- Windows 10 2004
- Windows 10 21H2
Inspired by the [smbdoor](https://github.com/loneicewolf/smbdoor) project

## Functional
At the moment, the backdoor has the ability to execute an arbitrary Usermode shellcode by injecting and creating a thread in the target process (explorer.exe by default). Custom calc shellcodes and shellcodes from Metasploit (stager) were tested, the shellcode size limit is 4000 bytes (it can be unstable with a larger size, but the limit can be changed in the code). It is enough to copy the shellcode to the shellcode.h file in the ping project (it must be in string format). If you want the shellcode to have SYSTEM privileges, then inject it in winlogon.exe. All functions are tested in x64, in x86 systems something may not work at all.

## Полезная литература / Useful information
https://blog.zecops.com/research/smbleedingghost-writeup-part-iii-from-remote-read-smbleed-to-rce

https://sec-in.com/article/1354

https://zhuanlan.zhihu.com/p/567559622
 
