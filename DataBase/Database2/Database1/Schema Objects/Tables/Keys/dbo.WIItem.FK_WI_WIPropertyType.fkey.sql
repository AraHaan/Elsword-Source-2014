﻿ALTER TABLE [dbo].[WIItem] ADD
CONSTRAINT [FK_WI_WIPropertyType] FOREIGN KEY ([PropertyType]) REFERENCES [dbo].[WIPropertyType] ([PropertyType]) ON DELETE CASCADE ON UPDATE CASCADE


