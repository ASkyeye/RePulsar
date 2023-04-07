# RePulsar
## Описание
Это PoC реализция SMB-бэкдора, основанная на собственном исследовании ядра Windows и механизма SMB. Так как это всего лишь PoC, особого функционала нету. В будущем планируется поддержка базового функционала:
| Функция  | Статус |
| -------- | ------- |
| APC инъекция кода  | :warning:   |
| WOW64LOG инъекция кода  | :warning:   |
| Чтение SMB тега | :heavy_check_mark:     |
| Прямое чтение/запись вирутальной памяти    | :x:    |
| Установка TCP-соединения   | :x:    |
:warning: - работа ведется, :heavy_check_mark: - функция завершена, :x: - работа пока не начата


Также планирутеся подробное описание работы данного механизма.
На текущий момент стабильно работает на:
- Windows 10 1809
- Windows 10 21H2

Вдохновлено проектом [smbdoor](https://github.com/loneicewolf/smbdoor)

## Description
This is a PoC implementation of an SMB backdoor based on our own research into the Windows kernel and the SMB mechanism. Since this is just a PoC, there is no special functionality. In the future, it is planned to support the basic functionality:
- Arbitrary read/write memory
- Shellcode execution
- Etc.

A detailed description of the operation of this mechanism is also planned.
Currently working stably on:
- Windows 10 1809
- Windows 10 21H2

Inspired by the [smbdoor](https://github.com/loneicewolf/smbdoor) project

## Полезная литература / Useful information
https://blog.zecops.com/research/smbleedingghost-writeup-part-iii-from-remote-read-smbleed-to-rce

https://sec-in.com/article/1354

https://zhuanlan.zhihu.com/p/567559622
 
