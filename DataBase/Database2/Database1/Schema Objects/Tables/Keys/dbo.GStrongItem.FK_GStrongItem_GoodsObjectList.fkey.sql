﻿ALTER TABLE [dbo].[GStrongItem] ADD
CONSTRAINT [FK_GStrongItem_GoodsObjectList] FOREIGN KEY ([ItemUID]) REFERENCES [dbo].[GoodsObjectList] ([ItemUID]) ON DELETE CASCADE ON UPDATE CASCADE


