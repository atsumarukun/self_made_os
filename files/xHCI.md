# xHCI 規格書 和訳

[xHCI 規格書](https://www.intel.com/content/dam/www/public/us/en/documents/technical-specifications/extensible-host-controler-interface-usb-xhci.pdf)の重要部分を和訳しまとめる。<br>
各セクションにおける(　)内の数字はxHCI規格書内で該当するセクション番号。
<br></br>

## 1. Host Controller Operatinal Registers (5.4)

| Offset    | Mnemonic | Register Name                             |
| --------- | -------- | ----------------------------------------- |
| 00h       | USBCMD   | USB Command                               |
| 04h       | USBSTS   | USB Status                                |
| 08h       | PAGESIZE | Page Size                                 |
| 0C-13h    | 予約     |                                           |
| 14h       | DNCTRL   | Device Notificattion Control              |
| 18h       | CRCR     | Command Ring Control                      |
| 20-2Fh    | 予約     |                                           |
| 30h       | DCBAAP   | Device Context Base Address Array Pointer |
| 38h       | CONFIG   | Configure                                 |
| 3C-3FFh   | 予約     |                                           |
| 400-13FFh |          | Port Register Set 1-MaxPorts              |

<br></br>

### 1.1. USB Command Register [USBCMD] (5.4.1)

| Bits  | Mnemonic                             | Descripttion |
| ----- | ------------------------------------ | ------------ |
| 0     | Run/Stop (RS)                        | '0'=Stop, '1'=Run.デフォルトは'0'.<br>このビットが'0'になると実行中及び実行待ちのトランザクションを完了し停止する.<br>USBSTSレジスタのHCHビットが'1'(停止)の時、このビットを'1'にしてはならない.<br>また、イベントリングがFullステートである時、このビットを'0'にしてはならない. |
| 1     | Host Controller Reset (HCRST)        | ホストコントローラをリセットする際に利用する.<br>このビットに'1'を書き込むと、ホストコントローラはタイマ等を初期化する.<br>USB2ではダウンストリームポートのリセットが行われないが、USB3では行われる.<br>PCIコンフィギュレーションレジスタはリセットの影響を受けない.<br>規格書のセクション4.2に従って再初期化を行う必要がある.<br>リセットが完了するとホストコントローラによってこのビットが'0'になる.<br>USBSTSレジスタのHCHビットが'0'の時、このビットを'1'にしてはならない. |
| 2     | Interrupter Enable (INTE)            | ホストシステム割り込みフラグのビット.<br>このビットが'1'の時割り込みが許可される(デフォルトは'0'). |
| 3     | Host System Error Enable (HSEE)      | このビットとUSBSTSレジスタのHSEビットが'1'の時、xHCはホストに対して帯域外エラー信号をアサートしなければならない. |
| 6:4   | 予約 ||
| 7     | Light Host Controller Reset (LHCRST) | オプションの規範的なビット. デフォルトは'0'.<br>HCCPARAMS1レジスタのLHRCビットが'1'の時、このフラグによりドライバはポートに影響を与えること無くxHCをリセットできる.<br>このビットが'0'の時、ライトホストストコントローラリセットが完了しxHCを最初期化することが安全であることを示す. |
| 8     | Conttroller Restore State (CRS)    |  |
| 9     |||
| 10    |||
| 11    |||
| 12    |||
| 13    |||
| 14    |||
| 15    |||
| 16    |||
| 31:17 | 予約 ||

<br></br>

## 2. Host Controller Initialization (4.2)

xHCを初期化するため、割り込みメカニズムとしてMSI-Xを利用した方法を以下に示す。

1. サポートされている場合、システムI/Oメモリマップを初期化する.
2. チップハードウェアリセットの後USBSTSレジスタのCNRビットが'0'になるまで待ち、xHCのOperationalレジスタまたはRuntimeレジスタに書き込む.
- ここ以降はUSBCMDのRun/Stopレジスタに1を書き込む前に完了する必要がある.
3. CONFIGレジスタのMaxSlotsEnフィールドを書き込む.
4. DCBAAPレジスタにDevice Context Base Address Arrayの64ビットアドレスを書き込む.
5. コマンドリングの開始アドレスを示す64bitアドレスをCRCRレジスタに書き込む.