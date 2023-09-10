ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    
    ;/*****Phase 1 : Prepare the LBA and sector count values:*****/
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive;This is done to indicate that the master drive is being selected.
    mov dx, 0x1F6;port to send LBA;The dx register is used to specify the I/O port that the hard disk controller is connected to.
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al;sending 24-31 bits 
    ;send al which contains number of sectors
    ; Finished sending the total sectors to read

    ;/*****Phase 2 : Sends commands and data to the storage device via specific ports:*****/
    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3;port to send LBA
    out dx, al ;sending 0-7 bits
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4;port to send LBA
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al ;sending 8-15 bits
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5;port to send LBA
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al;sending 16-23 bits
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7;command port; send command to HD controller.
    mov al, 0x20;0x20 is the command to start a read operation.
    out dx, al

    ;/*****Phase 3 : Waits for the storage device to be ready for data transfer:*****/
    ; Read all sectors into memory
    ;/** for each sector it will wait for HD controller to check if it is ready**/
.next_sector:
        push ecx

    ; Checking if we need to read
.try_again:
    mov dx, 0x1f7;The 0x1f7 I/O port is used to read the status of the hard disk controller.
    in al, dx
    test al, 8
    jz .try_again

    ;/*****Phase 4 : Transfers data from the storage device to memory:*****/
    ; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0;The 0x1F0 I/O port is used to read data from the hard disk.
    rep insw ;this will run 256 times; input word from i/o port specified in dx into memory location specified by es:di
    pop ecx;this will run 100 times, ecx-- => 100->99
    loop .next_sector
        ; End of reading sectors into memory
        ; now at the end we have read 256 words*100=25600 words = 51200bytes.
    ret