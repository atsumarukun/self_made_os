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
3. CONFIGレジスタのMaxSlotsEnフィールドを書き込む.
4. DCBAAPレジスタにDevice Context Base Address Arrayの64ビットアドレスを書き込む.
5. コマンドリングの開始アドレスを示す64bitアドレスをCRCRレジスタに書き込む.
6. 以下の方法で割り込みを初期化する.
    1. MSI-Xのメッセージテーブルを割り当て初期化、メッセージアドレスとメッセージデータの設定、ベクタを有効化する. 最低限テーブルベクタエントリ0を初期化し有効化しなければならない.
    2. MSI-Xの保留ビット配列を割り当て初期化する.
    3. MSI-X Capability StructureのTable OffsetとPBA OffsetsをそれぞれMSI-X Message Control TableとPending Bit Arrayに指定する.
    4. MSI-X Capability Structureのメッセージコントロールレジスタを初期化する.
    5. 以下の方法で各アクティブな割り込みを初期化する.
        - イベントリングの定義
            1. イベントリングセグメントの割当と初期化.
            2. イベントリングセグメントテーブル(ERST)を割り当てる. ERSTテーブルのエントリを初期化し、それぞれのイベントリングセグメントを示しサイズを定義する.
            3. ERSTSZレジスタにERSTテーブルに示したセグメント数を記す.
            4. ERDPレジスタにERSTテーブルに示された最初のセグメントの開始アドレスを記す.
            5. ERSTBAレジスタにERSTが配置される64ビットアドレスを示す.
            - ERSTBAレジスタに書き込むとイベントリングが有効化する.
        - 割り込みの定義
            1. MSI-X Capability Structure Message ControlレジスタのMSI-X Enableフラグを設定する.
            2. Interrupt Moderationレジスタのintervalフィールドをターゲット割り込みモデレーションレートで初期化する.
            3. USBCMDレジスタのINTEビットに'1'を書き込む.
            4. Interrupter ManagementレジスタのIEフィールドに'1'を書き込む.
7. USBCMDレジスタのRun/Stopビットに'1'を書き込む.