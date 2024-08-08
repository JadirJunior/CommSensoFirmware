## Endereçamentos do sensor NPK

| Variável        | Array de Bytes                                   |
|-----------------|--------------------------------------------------|
| temperatura     | `{0x01, 0x03, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00}` |
| umidade         | `{0x01, 0x03, 0x02, 0x01, 0x00, 0x01, 0x00, 0x00}` |
| nitrogenio      | `{0x01, 0x03, 0x02, 0x04, 0x00, 0x01, 0x00, 0x00}` |
| fosforo         | `{0x01, 0x03, 0x02, 0x05, 0x00, 0x01, 0x00, 0x00}` |
| Ph              | `{0x01, 0x03, 0x02, 0x03, 0x00, 0x01, 0x00, 0x00}` |
| condutividade   | `{0x01, 0x03, 0x02, 0x02, 0x00, 0x01, 0x00, 0x00}` |
| potassio        | `{0x01, 0x03, 0x02, 0x06, 0x00, 0x01, 0x00, 0x00}` |


## Mapeamento de Pinos

| Nome    | Pino | Tipo   | Descrição                                                                                   |
|---------|------|--------|---------------------------------------------------------------------------------------------|
| RE      | 19   | OUTPUT | Pino RE do módulo RS485, podendo ser qualquer GPIO do ESP                                    |
| DE      | 18   | OUTPUT | Pino DE do módulo RS485, podendo ser qualquer GPIO do ESP                                    |
| PIN_RX  | 16   | INPUT  | Pino RX do ESP conectado ao RO do módulo para receber os dados                               |
| PIN_TX  | 17   | OUTPUT | Pino TX do ESP conectado ao DI do módulo para enviar os dados                                |
