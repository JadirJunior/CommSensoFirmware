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


## Mapeamento de pinos

| Nome | Pino | Tipo   | Descrição                                     |
|------|------|--------|-----------------------------------------------|
| RE   | 19   | OUTPUT | Pino RE do módulo RS485, podendo ser qualquer GPIO do ESP |
| DE   | 18   | OUTPUT | Pino DE do módulo RS485, podendo ser qualquer GPIO do ESP |
| RO   | RX2  | INPUT  | Pino para o ESP receber os dados              |
| DI   | TX2  | OUTPUT | Pino para o ESP enviar os dados               |

