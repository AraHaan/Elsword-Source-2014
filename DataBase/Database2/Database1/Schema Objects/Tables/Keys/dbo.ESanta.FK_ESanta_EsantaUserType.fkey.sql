﻿ALTER TABLE [dbo].[ESanta] ADD
CONSTRAINT [FK_ESanta_EsantaUserType] FOREIGN KEY ([UserType]) REFERENCES [dbo].[ESantaUserType] ([UserType]) ON DELETE CASCADE ON UPDATE CASCADE


